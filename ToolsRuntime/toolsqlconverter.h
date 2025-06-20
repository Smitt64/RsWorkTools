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

class TOOLSRUNTIME_EXPORT SqlConverter : public QObject
{
    Q_OBJECT
public:
    explicit SqlConverter(QObject *parent = nullptr);
    virtual ~SqlConverter();

signals:

};

TOOLSRUNTIME_EXPORT SqlConversionResult convertSql(const QString &sql);

#endif // TOOLSQLCONVERTER_H
