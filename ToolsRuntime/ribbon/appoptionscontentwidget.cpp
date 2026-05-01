#include "appoptionscontentwidget.h"
#include "optionsdlg/OptionsPage.h"
#include "optionsdlg/rsloptionspage.h"
#include "optionsdlg/logoptionspage.h"
#include "toolsruntime.h"
#include "widgets/stringlisteditor.h"
#include <QSettings>
#include <QToolButton>
#include <QDebug>

class AppOptionsContentWidgetPrivate
{
    Q_DECLARE_PUBLIC(AppOptionsContentWidget)
public:
    AppOptionsContentWidgetPrivate(AppOptionsContentWidget *obj)
    {
        q_ptr = obj;
        m_ScrollArea = new QScrollArea();

        // Создаем контейнер для кнопок с горизонтальным layout
        m_pButtonContainer = new QWidget();
        QHBoxLayout *buttonLayout = new QHBoxLayout(m_pButtonContainer);
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        buttonLayout->setSpacing(10);

        // Кнопка "Применить"
        m_pApplyButton = new QToolButton();
        m_pApplyButton->setText(q_ptr->tr("Применить"));
        m_pApplyButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_pApplyButton->setIcon(QIcon::fromTheme("DocumentOK"));
        m_pApplyButton->setIconSize(QSize(32, 32));
        m_pApplyButton->setFixedSize(82, 82);
        toolAddActionWithTooltip(m_pApplyButton, q_ptr->tr("Применить все изменения в настройках"));

        // Кнопка "Отменить"
        m_pCancelButton = new QToolButton();
        m_pCancelButton->setText(q_ptr->tr("Отменить"));
        m_pCancelButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_pCancelButton->setIcon(QIcon::fromTheme("DocumentExclude"));
        m_pCancelButton->setIconSize(QSize(32, 32));
        m_pCancelButton->setFixedSize(82, 82);
        toolAddActionWithTooltip(m_pCancelButton, q_ptr->tr("Отменить все несохраненные изменения"));

        buttonLayout->addWidget(m_pApplyButton);
        buttonLayout->addWidget(m_pCancelButton);
        buttonLayout->addStretch(); // Растягивающийся spacer справа

        // Добавляем отступ под кнопками
        m_pButtonContainer->setContentsMargins(0, 0, 0, 10);
    }

    void itemClicked(QListWidgetItem *item, const int &row)
    {
        Q_Q(AppOptionsContentWidget);
        (void)m_ScrollArea->takeWidget();
        m_ScrollArea->setWidget(m_Pages[row]);
    }

    void applySettings()
    {
        for (OptionsPage *page : std::as_const(m_Pages))
        {
            if (m_pSettings)
            {
                (void)page->save();
                m_pSettings->sync();
            }
        }
    }

    void cancelSettings()
    {
        for (OptionsPage *page : std::as_const(m_Pages))
        {
            if (m_pSettings)
            {
                (void)page->restore();
                m_pSettings->sync();
            }
        }
    }

    void ConfigureStringListEditor(StringListEditor *_editor)
    {
        if (!_editor)
            return;

        StdFolderListHandler *_Handler = dynamic_cast<StdFolderListHandler*>(_editor->handler());
        //_Handler->_size = QSize(0, 22);
        _Handler->_rowIcon = QIcon::fromTheme("FolderClosed");
        _Handler->_buttonIcon = QIcon::fromTheme("OpenFolder");

        QAbstractButton* newBtn = _editor->button(StringListEditor::ButtonNew);
        newBtn->setIcon(QIcon::fromTheme("Create"));
        newBtn->setIconSize(QSize(24, 24));
        toolAddActionWithTooltip(newBtn,  _editor->tr("Создать новую запись"));

        QAbstractButton* addFolderBtn = _editor->button(StringListEditor::ButtonAddFolder);
        addFolderBtn->setIcon(QIcon::fromTheme("FolderOpened"));
        addFolderBtn->setIconSize(QSize(24, 24));
        toolAddActionWithTooltip(addFolderBtn, _editor->tr("Выбрать папку из файловой системы"));

        QAbstractButton* removeBtn = _editor->button(StringListEditor::ButtonRemove);
        removeBtn->setIcon(QIcon::fromTheme("Remove_color"));
        removeBtn->setIconSize(QSize(24, 24));
        toolAddActionWithTooltip(removeBtn,_editor->tr("Удалить выбранную запись"));

        QListView *view = _editor->findChild<QListView*>();
        view->setFrameShape(QFrame::NoFrame);
    }

    QWidget *m_pButtonContainer;
    QToolButton *m_pApplyButton;
    QToolButton *m_pCancelButton;
    QScrollArea *m_ScrollArea;
    QSettings *m_pSettings;
    QList<OptionsPage*> m_Pages;
    AppOptionsContentWidget *q_ptr;
};

AppOptionsContentWidget::AppOptionsContentWidget(QWidget *parent) :
    CategoryContentWidget(parent),
    d_ptr(new AppOptionsContentWidgetPrivate(this))
{
    Q_D(AppOptionsContentWidget);
    setLayoutType(CategoryContentWidget::LayoutListDetail);

    addWidgetBeforeList(d->m_pButtonContainer);
    setDetailWidget(d->m_ScrollArea);

    // Не нужно отдельно устанавливать стиль, он применится через CategoryContentWidget

    connect(d->m_pApplyButton, &QToolButton::clicked, this, [d]() {
        d->applySettings();
    });

    connect(d->m_pCancelButton, &QToolButton::clicked, this, [d]() {
        d->cancelSettings();
    });

    connect(listWidget(), &QListWidget::itemClicked, [=](QListWidgetItem *item) {
        d->itemClicked(item, listWidget()->row(item));
    });
}

AppOptionsContentWidget::~AppOptionsContentWidget()
{
    delete d_ptr;
}

void AppOptionsContentWidget::setSettings(QSettings *pSettings)
{
    Q_D(AppOptionsContentWidget);
    d->m_pSettings = pSettings;

    for (OptionsPage* page : qAsConst(d->m_Pages))
    {
        page->setSettings(pSettings);
        page->restore();
    }
}

void AppOptionsContentWidget::deletePage(const int &row)
{
    Q_D(AppOptionsContentWidget);
    OptionsPage *page = d->m_Pages.takeAt(row);
    delete page;

    deleteListItem(row);
}

QList<OptionsPage*> AppOptionsContentWidget::pages()
{
    Q_D(AppOptionsContentWidget);
    return d->m_Pages;
}

int AppOptionsContentWidget::addPage(const QString &title, const QIcon &icon, OptionsPage *page)
{
    Q_D(AppOptionsContentWidget);

    int pos = d->m_Pages.size();

    addListItem(title, icon);
    d->m_Pages.append(page);

    if (!listWidget()->currentItem())
        listWidget()->setCurrentRow(0);

    d->itemClicked(listWidget()->currentItem(), 0);

    return pos;
}

int AppOptionsContentWidget::addPage(int row, const QString &title, const QIcon &icon, OptionsPage *page)
{
    Q_D(AppOptionsContentWidget);

    int pos = d->m_Pages.size();

    insertListItem(row, title, icon);
    d->m_Pages.insert(row, page);

    if (!listWidget()->currentItem())
        listWidget()->setCurrentRow(0);

    d->itemClicked(listWidget()->currentItem(), 0);

    return pos;
}

int AppOptionsContentWidget::addRslPage(const QIcon &icon)
{
    Q_D(AppOptionsContentWidget);

    RslOptionsPage *page = new RslOptionsPage();
    StringListEditor *_editor = page->findChild<StringListEditor*>();
    d->ConfigureStringListEditor(_editor);

    return addPage(tr("Rsl интерпретатор"), icon, page);
}

int AppOptionsContentWidget::addLogPage(const QIcon &icon, const QString &prefix)
{
    LogOptionsPage *page = new LogOptionsPage();
    page->setPrefix(prefix);
    page->setLogButtonIcon(QIcon::fromTheme("LogProvider"));

    QTreeView* tree = page->findChild<QTreeView*>();
    tree->setFrameShape(QFrame::NoFrame);

    QToolButton* toolButton = page->findChild<QToolButton*>();
    if (toolButton)
        toolAddActionWithTooltip(toolButton, tr("Открыть папку с файлом лога"));

    return addPage(tr("Логирование"), icon, page);
}

void AppOptionsContentWidget::configureStringListEditor(StringListEditor *_editor)
{
    Q_D(AppOptionsContentWidget);
    d->ConfigureStringListEditor(_editor);
}

void AppOptionsContentWidget::configureStringListEditors(QWidget *container)
{
    Q_D(AppOptionsContentWidget);
    QList<StringListEditor*> _editors = container->findChildren<StringListEditor*>();

    for (StringListEditor* _editor : qAsConst(_editors))
        d->ConfigureStringListEditor(_editor);
}