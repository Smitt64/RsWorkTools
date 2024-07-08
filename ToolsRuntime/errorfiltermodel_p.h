#ifndef ERRORFILTERMODEL_P_H
#define ERRORFILTERMODEL_P_H

#include <QSortFilterProxyModel>

class ErrorsModel;
class ErrorFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ErrorFilterModel(QObject *parent = 0);
    ~ErrorFilterModel();

    void setFmtErrors(ErrorsModel *e);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public slots:
    void setShowInformation(bool value);
    void setShowWarnings(bool value);
    void setShowErrors(bool value);

private:
    bool m_ShowInformation, m_ShowWarnings, m_ShowErrors;
    ErrorsModel *pErrors;
};

#endif // ERRORFILTERMODEL_P_H
