#include "iconlibdlg.h"
#include "ui_iconlibdlg.h"
#include <QAbstractTableModel>
#include <QDirIterator>
#include <QFileInfo>
#include <QPushButton>
#include <QSize>
#include <QDebug>

QIcon resizeIcon(const QIcon& icon) 
{
    QSize size = icon.actualSize(QSize(32, 32));

    if (size == QSize(32, 32))
        return icon;

    QPixmap pixmap = icon.pixmap(size);
    pixmap = pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return QIcon(pixmap);
}

class IconListModel : public QAbstractTableModel
{
    typedef struct 
    {
        QIcon icon;
        QString name;
        QString path;
    }IconInfo;
    
public:
    IconListModel(QObject *parent) : 
        QAbstractTableModel(parent)
    {
        QDirIterator it(":/iconlib", QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString path = it.next();
            QFileInfo file(path);

            IconInfo info;
            info.name = file.baseName();
            info.name[0] = info.name[0].toUpper();
            info.path = path;
            info.icon = resizeIcon(QIcon(path));
            m_list.append(info);
        }
        
        qSort(m_list.begin(), m_list.end(), [=](const IconInfo &s1, const IconInfo &s2)
        {
            return s1.name < s2.name;
        });
    }

    virtual ~IconListModel() 
    {

    }

    int rowCount(const QModelIndex &parent) const
    {
        return m_list.size();
    }

    int columnCount(const QModelIndex &parent) const
    {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
            return m_list[index.row()].name;
        else if (role == Qt::UserRole)
            return m_list[index.row()].path;
        else if (role == Qt::DecorationRole)
            return m_list[index.row()].icon;
        else if (role == Qt::SizeHintRole)
            return QSize(80, 80);

        return QVariant();
    }

private:
    QList<IconInfo> m_list;
};

IconLibDlg::IconLibDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IconLibDlg)
{
    ui->setupUi(this);
    m_pModel = new IconListModel(this);
    ui->listView->setModel(m_pModel);
    ui->listView->setWordWrap(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->listView, &QListView::clicked, [=](const QModelIndex &index)
    {
        ui->aliasLabel->setText(QString("Элемент: %1")
                                .arg(index.data().toString()));
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(index.isValid());
    });

    connect(ui->listView, &QListView::doubleClicked, [=](const QModelIndex &index)
    {
        accept();
    });
}

IconLibDlg::~IconLibDlg()
{
    delete ui;
}

QString IconLibDlg::getIconPath() const
{
    return m_pModel->data(ui->listView->currentIndex(), Qt::UserRole).toString();
}
