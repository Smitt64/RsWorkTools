#ifndef SVNSTATUSMODEL_H
#define SVNSTATUSMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include "ToolsRuntime_global.h"

typedef struct
{
    QString fullpath, path, author, filename;
    QString action, revision, props;
    QDateTime date;
}SvnStatusElement;

class TOOLSRUNTIME_EXPORT SvnStatusModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        fld_FullFile = 0,
        fld_File,
        fld_FileName,
        fld_Action,

        fld_Count
    };
    explicit SvnStatusModel(QObject *parent = nullptr);
    virtual ~SvnStatusModel();

    void setPath(const QString &path, const QString &revision);

    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    const SvnStatusElement &element(const int &row) const;
private:
    QList<SvnStatusElement> m_Elements;
    QString m_Path;
};

#endif // SVNSTATUSMODEL_H
