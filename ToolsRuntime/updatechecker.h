#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QRunnable>
#include "ToolsRuntime_global.h"

typedef struct
{
    QString id;
    QString name;
    QString version;
    QString sizeString;
    qint64 size;
}CheckUpdateData;
typedef QList<CheckUpdateData> CheckDataList;

class UpdateCheckerPrivate;
class TOOLSRUNTIME_EXPORT UpdateChecker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);
    virtual ~UpdateChecker();

    void setProgramName(const QString &name);
    void requestInterruption();
    void setCheckUpdateFlag(bool value);

    void run() Q_DECL_OVERRIDE;

signals:
    void checkFinished(bool hasUpdates, const CheckDataList &updatedata = CheckDataList());
    void checkStarted();

private:
    UpdateCheckerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(UpdateChecker);
};

Q_DECLARE_METATYPE(CheckUpdateData)
Q_DECLARE_METATYPE(CheckDataList)

#endif // UPDATECHECKER_H
