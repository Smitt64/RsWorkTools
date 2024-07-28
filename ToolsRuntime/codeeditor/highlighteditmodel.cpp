#include "highlighteditmodel.h"
#include "codeeditor/highlighterstyle.h"
#include "qapplication.h"
#include "qpainter.h"
#include "toolsruntime.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QTextCursor>
#include <QApplication>
#include <QDebug>

class ItemDelegate : public QStyledItemDelegate
{
public:
    ItemDelegate(QObject *parent = nullptr) :
        QStyledItemDelegate(parent)
    {

    }

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize sz = option.rect.size();
        sz.setHeight(24);
        return sz;
    }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        HighlightEditModel *model = (HighlightEditModel*)index.model();

        QTextDocument document;
        QTextCursor cur(&document);
        cur.setBlockCharFormat(model->format(index.row()));
        cur.insertText(index.data().toString());

        QStyleOptionViewItem opt = option;
        opt.text = QString();
        opt.backgroundBrush = QBrush(model->editorBackground());

        QStyle *style = QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

        QPixmap tmp(option.rect.size());
        tmp.fill(Qt::transparent);
        QPainter tmpp(&tmp);

        document.drawContents(&tmpp);
        painter->drawPixmap(option.rect, tmp);
    }
};

typedef struct
{
    QString name;
    QString stylealias;

    //QTextCharFormat StyleParam;
}HighlightEditModelData;

class HighlightEditModelPrivate
{
    Q_DECLARE_PUBLIC(HighlightEditModel)
public:
    HighlightEditModelPrivate(HighlightEditModel *parent)
    {
        q_ptr = parent;
        pDelegate = new ItemDelegate(parent);
        loadList();
    }

    void loadList()
    {
        QJsonParseError err;
        QByteArray content = toolReadFileContent(":/codeeditoroptions/ListParams.json");
        QJsonDocument doc = QJsonDocument::fromJson(content, &err);
        QJsonObject root = doc.object();

        QJsonArray arr = root["aliases"].toArray();

        for (const QJsonValue &val : arr)
        {
            QJsonObject obj = val.toObject();

            HighlightEditModelData data;
            data.name = obj["listalias"].toString();
            data.stylealias = obj["alias"].toString();

            m_Data.append(data);
        }
    }

    QSharedPointer<StyleItem> style;
    QColor editorBackground, editorCurrentLine, editorCurrentWord;
    QColor linenumbersBackground, linenumbersForeground;
    ItemDelegate *pDelegate;
    QList<HighlightEditModelData> m_Data;
    HighlightEditModel *q_ptr;
};

HighlightEditModel::HighlightEditModel(QObject *parent)
    : QAbstractItemModel(parent),
    d_ptr(new HighlightEditModelPrivate(this))
{

}

HighlightEditModel::~HighlightEditModel()
{
    Q_D(HighlightEditModel);
    delete d_ptr;
}

QAbstractItemDelegate *HighlightEditModel::delegate()
{
    Q_D(HighlightEditModel);
    return d->pDelegate;
}

QTextCharFormat &HighlightEditModel::format(const int &format) const
{
    Q_D(const HighlightEditModel);
    return d->style->format(d->m_Data[format].stylealias);
}

const QColor &HighlightEditModel::editorBackground() const
{
    Q_D(const HighlightEditModel);
    return d->editorBackground;
}

const QColor &HighlightEditModel::editorCurrentLine() const
{
    Q_D(const HighlightEditModel);
    return d->editorCurrentLine;
}

const QColor &HighlightEditModel::editorCurrentWord() const
{
    Q_D(const HighlightEditModel);
    return d->editorCurrentWord;
}

const QColor &HighlightEditModel::linenumbersBackground() const
{
    Q_D(const HighlightEditModel);
    return d->linenumbersBackground;
}

const QColor &HighlightEditModel::linenumbersForeground() const
{
    Q_D(const HighlightEditModel);
    return d->linenumbersForeground;
}

QSharedPointer<StyleItem> HighlightEditModel::style()
{
    Q_D(HighlightEditModel);
    return d->style;
}

void HighlightEditModel::setStyle(const QString &s)
{
    Q_D(HighlightEditModel);

    QSharedPointer<StyleItem> style = HighlighterStyle::inst()->style(s);

    QColor editorBackground = style->editorBackground();

    if (editorBackground.isValid())
        d->editorBackground = editorBackground;
    else
        d->editorBackground = qApp->palette().color(QPalette::Base);

    d->editorCurrentLine = style->editorCurrentLine();
    d->editorCurrentWord = style->editorCurrentWord();

    d->linenumbersBackground = style->linenumbersBackground();
    d->linenumbersForeground = style->linenumbersForeground();

    d->style = style;
}

QVariant HighlightEditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

bool HighlightEditModel::setHeaderData(int section,
                                       Qt::Orientation orientation,
                                       const QVariant &value,
                                       int role)
{
    return false;
}

QModelIndex HighlightEditModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex HighlightEditModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int HighlightEditModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const HighlightEditModel);
    return d->m_Data.size();
}

int HighlightEditModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

bool HighlightEditModel::hasChildren(const QModelIndex &parent) const
{
    return false;
}

bool HighlightEditModel::canFetchMore(const QModelIndex &parent) const
{
    return false;
}

void HighlightEditModel::fetchMore(const QModelIndex &parent)
{
}

QVariant HighlightEditModel::data(const QModelIndex &index, int role) const
{
    Q_D(const HighlightEditModel);
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return d->m_Data[index.row()].name;

    return QVariant();
}

bool HighlightEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags HighlightEditModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

void HighlightEditModel::reset()
{
    beginResetModel();
    endResetModel();
}
