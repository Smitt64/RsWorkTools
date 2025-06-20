#ifndef TOOLSQLCONVERTER_H
#define TOOLSQLCONVERTER_H

#include <QObject>
#include "ToolsRuntime_global.h"

typedef struct
{
    QString result;
    QString error;
    QString tail;
} SqlConversionResult;

class SqlConverterPrivate;
class TOOLSRUNTIME_EXPORT SqlConverter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QString tail READ tail NOTIFY tailChanged)

public:
    Q_INVOKABLE SqlConverter(QObject *parent = nullptr);
    virtual ~SqlConverter();

    Q_INVOKABLE QString convert(const QString &sql);
    QString lastError() const;
    QString tail() const;

signals:
    void lastErrorChanged(const QString &error);
    void tailChanged(const QString &tail);

private:
    QScopedPointer<SqlConverterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(SqlConverter)
};

TOOLSRUNTIME_EXPORT SqlConversionResult convertSql(const QString &sql);

#endif // TOOLSQLCONVERTER_H
