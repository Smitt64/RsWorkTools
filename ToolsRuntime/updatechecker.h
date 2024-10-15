// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QRunnable>
#include "ToolsRuntime_global.h"

enum CheckUpdateDataType
{
    UpdateType_Record = 0,
    UpdateType_Critical,
    UpdateType_Warning
};

typedef struct
{
    QString id;
    QString name;
    QString version;
    QString sizeString;
    qint64 size;

    int type;
}CheckUpdateData;
typedef QList<CheckUpdateData> CheckDataList;

class QSettings;
class QStandardItemModel;
class UpdateCheckerPrivate;
class TOOLSRUNTIME_EXPORT UpdateChecker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);
    virtual ~UpdateChecker();

    void setSettings(QSettings *prm);
    void setProgramName(const QString &name);
    void requestInterruption();
    void setCheckUpdateFlag(bool value);
    void setInterval(int msec);

    static void MakeUpdateModel(QStandardItemModel **model, const CheckDataList &lst, QObject *parent = nullptr);

    void checkUpdate(CheckDataList *updatedata);
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
