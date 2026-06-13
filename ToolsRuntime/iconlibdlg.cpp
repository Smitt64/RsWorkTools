#include "iconlibdlg.h"
#include "ui_iconlibdlg.h"
#include "iconthememanager.h"
#include <QAbstractTableModel>
#include <algorithm>
#include <QDirIterator>
#include <QFileInfo>
#include <QPushButton>
#include <QSize>
#include <QDebug>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QApplication>

QIcon resizeIcon(const QIcon& icon)
{
    QPixmap pixmap = icon.pixmap(QSize(32, 32));
    if (pixmap.size() != QSize(32, 32))
        pixmap = pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return QIcon(pixmap);
}

// Делегат, который гарантирует одинаковый размер ячейки и центрирование
// иконки с текстом внутри неё.
class IconItemDelegate : public QStyledItemDelegate
{
public:
    explicit IconItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save();

        const QRect &rect = option.rect;

        // Фон выделения / наведения — рисуем сами, чтобы размер всегда совпадал с option.rect
        if (option.state & QStyle::State_Selected)
            painter->fillRect(rect, option.palette.highlight());
        else if (option.state & QStyle::State_MouseOver)
            painter->fillRect(rect, option.palette.alternateBase());

        QRect contentRect = rect.adjusted(4, 4, -4, -4);

        // Иконка строго по центру в верхней части ячейки
        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
        QSize iconSize = option.decorationSize.isValid() ? option.decorationSize : QSize(32, 32);
        QPixmap pixmap = icon.pixmap(iconSize);
        QRect iconRect = pixmap.rect();
        iconRect.moveCenter(QPoint(contentRect.center().x(), contentRect.top() + pixmap.height() / 2 + 2));
        painter->drawPixmap(iconRect, pixmap);

        // Текст по центру в нижней части ячейки
        QString text = index.data(Qt::DisplayRole).toString();
        QRect textRect = contentRect;
        textRect.setTop(iconRect.bottom() + 4);

        painter->setPen(option.state & QStyle::State_Selected
                            ? option.palette.color(QPalette::HighlightedText)
                            : option.palette.color(QPalette::Text));
        painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, text);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        return QSize(90, 90);
    }
};

// Возвращает абсолютный путь к корневой директории текущей темы иконок
// (поиск по путям, зарегистрированным в IconThemeManager / QIcon).
static QString currentThemeDirectory()
{
    const QString themeName = IconThemeManager::themeName();
    if (themeName.isEmpty())
        return QString();

    for (const QString &searchPath : IconThemeManager::currentSearchPaths())
    {
        QDir dir(QDir(searchPath).absoluteFilePath(themeName));
        if (dir.exists())
            return dir.absolutePath();
    }
    return QString();
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
        loadIconsFromTheme();
        if (m_list.isEmpty())
            loadIconsFromResources();

        std::sort(m_list.begin(), m_list.end(), [=](const IconInfo &s1, const IconInfo &s2)
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
            return QSize(90, 90);
        else if (role == Qt::TextAlignmentRole)
            return int(Qt::AlignCenter);

        return QVariant();
    }

private:
    void loadIconsFromTheme()
    {
        const QString themeDir = currentThemeDirectory();
        if (themeDir.isEmpty())
            return;

        QDirIterator it(themeDir,
                        QStringList() << "*.svg" << "*.png" << "*.xpm",
                        QDir::Files,
                        QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString path = it.next();
            QFileInfo file(path);

            IconInfo info;
            info.name = file.baseName();
            if (!info.name.isEmpty())
                info.name[0] = info.name[0].toUpper();
            info.path = path;
            info.icon = resizeIcon(QIcon(path));
            m_list.append(info);
        }
    }

    void loadIconsFromResources()
    {
        QDirIterator it(":/iconlib", QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString path = it.next();
            QFileInfo file(path);
            if (file.isDir())
                continue;

            IconInfo info;
            info.name = file.baseName();
            if (!info.name.isEmpty())
                info.name[0] = info.name[0].toUpper();
            info.path = path;
            info.icon = resizeIcon(QIcon(path));
            m_list.append(info);
        }
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
    ui->listView->setIconSize(QSize(32, 32));
    ui->listView->setItemDelegate(new IconItemDelegate(ui->listView));
    ui->listView->setUniformItemSizes(true);
    ui->listView->setResizeMode(QListView::Fixed);
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
