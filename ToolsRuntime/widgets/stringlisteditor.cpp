#include "stringlisteditor.h"
#include "ui_stringlisteditor.h"
#include <QStandardItemModel>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QDebug>
#include <QFileDialog>
#include <QDir>

QIcon StdFolderListHandler::buttonIcon() const
{
    return QIcon(":/img/openfolderHS.png");
}

bool StdFolderListHandler::click(QString &text, QWidget *parent)
{
    QString path = QFileDialog::getExistingDirectory(parent);

    if (path.isEmpty())
        return false;

    QDir appdir(qApp->applicationDirPath());
    text = appdir.relativeFilePath(path);

    return true;
}

class EditableJarDelegate : public QStyledItemDelegate
{
public:
    EditableJarDelegate(QObject *parent = nullptr) :
        QStyledItemDelegate(parent),
        m_pCurrent(nullptr),
        m_IsOpenDialog(false)
    {
        m_pHandler = nullptr;
    }

    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        EditableJarDelegate *pThis = const_cast<EditableJarDelegate*>(this);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);

        QWidget *frame = new QWidget(parent);
        frame->setLayout(layout);
        frame->setFocusPolicy(Qt::StrongFocus);
        frame->setObjectName("frame");

        QLineEdit *edit = new QLineEdit(frame);
        edit->setObjectName("lineEdit");

        QPushButton *btn = new QPushButton();
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setFlat(true);

        btn->setFixedSize(option.rect.height(), option.rect.height());
        btn->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(edit);
        layout->addWidget(btn);

        if (!m_pHandler)
            btn->setVisible(false);
        else
            btn->setIcon(m_pHandler->buttonIcon());

        connect(btn, &QPushButton::clicked, [=]() -> void
        {
            pThis->m_IsOpenDialog = true;
            QString text = edit->text();

            if (m_pHandler->click(text, parent))
                edit->setText(text);

            edit->setFocus();
            pThis->m_IsOpenDialog = false;
        });

        frame->setFocusProxy(edit);
        edit->installEventFilter(pThis);

        pThis->m_pCurrent = frame;
        pThis->m_CurrentIndex = index;
        return frame;
    }

    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE
    {
        if (event->type() == QEvent::FocusOut)
        {
            if (!qstrcmp(obj->metaObject()->className(), "QLineEdit") && !m_IsOpenDialog)
            {
                QListView *view = qobject_cast<QListView*>(parent());
                setModelData(m_pCurrent, view->model(), m_CurrentIndex);
                emit closeEditor(m_pCurrent, QAbstractItemDelegate::SubmitModelCache);
            }

            return true;
        }

        return QStyledItemDelegate::eventFilter(obj, event);
    }

    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QLineEdit *edit = editor->findChild<QLineEdit*>();

        if (!edit)
            return;

        model->setData(index, edit->text().trimmed());
    }

    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QLineEdit *edit = editor->findChild<QLineEdit*>();

        if (!edit)
            return;

        edit->setText(index.data().toString());
    }

    QWidget *m_pCurrent;
    QModelIndex m_CurrentIndex;
    StringListEditorHandler *m_pHandler;

    bool m_IsOpenDialog;
};

// -------------------------------------------------------------

class StringListEditorPrivate
{
    Q_DECLARE_PUBLIC(StringListEditor)
public:
    StringListEditorPrivate(StringListEditor *parent)
    {
        //Q_Q(StringListEditor);
        q_ptr = parent;
        m_pHandler = nullptr;
    }

    StringListEditorHandler *m_pHandler;
    QStandardItemModel *m_pModel;
    EditableJarDelegate *m_pDelegate;
    StringListEditor *q_ptr;

    QStringList m_StaticStrings;
};

StringListEditor::StringListEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StringListEditor)
    , d_ptr(new StringListEditorPrivate(this))
{
    Q_D(StringListEditor);
    ui->setupUi(this);

    d->m_pModel = new QStandardItemModel(parent);
    d->m_pModel->setColumnCount(1);
    d->m_pDelegate = new EditableJarDelegate(ui->listView);
    ui->listView->setModel(d->m_pModel);
    ui->listView->setItemDelegate(d->m_pDelegate);

    ui->addFilesBtn->setVisible(false);

    connect(ui->newBtn, &QPushButton::clicked, [=]()
    {
        int rowcount = d->m_pModel->rowCount();
        d->m_pModel->insertRow(rowcount);
        ui->listView->edit(d->m_pModel->index(rowcount, 0));
    });

    connect(ui->remBtn, &QPushButton::clicked, [=]()
    {
        QItemSelectionModel *selection = ui->listView->selectionModel();
        QModelIndex index = selection->currentIndex();

        if (!index.isValid())
            return;

        d->m_pModel->removeRow(index.row());
    });
}

StringListEditor::~StringListEditor()
{
    delete ui;
}

void StringListEditor::addStatic(const QString &value)
{
    Q_D(StringListEditor);

    QStandardItem *item = new QStandardItem();
    item->setEnabled(false);
    item->setText(value);
    item->setSelectable(false);

    d->m_StaticStrings.append(value);
    d->m_pModel->appendRow(item);
}

void StringListEditor::addStatic(const QStringList &lst)
{
    for (const QString &file : lst)
        addStatic(file);
}

void StringListEditor::addList(const QStringList &lst)
{
    Q_D(StringListEditor);

    for (const QString &file : lst)
    {
        QStandardItem *item = new QStandardItem();
        item->setText(file);

        d->m_pModel->appendRow(item);
    }
}

QStringList StringListEditor::stringList()
{
    Q_D(StringListEditor);

    QStringList files;
    for (int i = 0; i < d->m_pModel->rowCount(); i++)
    {
        QStandardItem *item = d->m_pModel->item(i);

        if (item->isEditable() && item->isEditable() && !d->m_StaticStrings.contains(item->text()))
            files.append(item->text());
    }

    return files;
}

QAbstractButton *StringListEditor::button(StandartButtons btn)
{
    QAbstractButton *b = nullptr;

    switch(btn)
    {
    case ButtonNew:
        b = ui->newBtn;
        break;
    case ButtonAddFolder:
        b = ui->addFilesBtn;
        break;
    case ButtonRemove:
        b = ui->remBtn;
        break;
    }

    return b;
}

void StringListEditor::append(const QString &value)
{
    Q_D(StringListEditor);

    QStandardItem *item = new QStandardItem();
    item->setText(value);

    d->m_pModel->appendRow(item);
}

void StringListEditor::append(const QStringList &lst)
{
    addList(lst);
}

void StringListEditor::setHandler(StringListEditorHandler *handler)
{
    Q_D(StringListEditor);
    d->m_pHandler = handler;
    d->m_pDelegate->m_pHandler = handler;
}
