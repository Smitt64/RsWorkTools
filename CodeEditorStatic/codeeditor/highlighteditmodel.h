#ifndef HIGHLIGHTEDITMODEL_H
#define HIGHLIGHTEDITMODEL_H

#include "qabstractitemdelegate.h"
#include <QAbstractItemModel>

class QTextCharFormat;
class QStyledItemDelegate;
class HighlightEditModelPrivate;
class StyleItem;
class HighlightEditModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    HighlightEditModel(QObject *parent = nullptr);
    virtual ~HighlightEditModel();

    void setStyle(const QString &s);
    QSharedPointer<StyleItem> style();
    QAbstractItemDelegate *delegate();
    QTextCharFormat &format(const int &format) const;

    const QColor &editorBackground() const;
    const QColor &editorCurrentLine() const;
    const QColor &editorCurrentWord() const;

    const QColor &linenumbersBackground() const;
    const QColor &linenumbersForeground() const;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void reset();

private:
    HighlightEditModelPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(HighlightEditModel);
};

#endif // HIGHLIGHTEDITMODEL_H
