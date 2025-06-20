#ifndef PROPERTYWIDGETMAPPER_H
#define PROPERTYWIDGETMAPPER_H

#include <QObject>
#include "ToolsRuntime_global.h"

class QAction;
class TOOLSRUNTIME_EXPORT PropertyWidgetMapper : public QObject
{
    Q_OBJECT
public:
    explicit PropertyWidgetMapper(QObject *parent = nullptr);
    virtual ~PropertyWidgetMapper() = default;

    // Привязка Q_PROPERTY к QAction (например, checked)
    bool bind(QObject *source, const char *property, QAction *action);

    // Привязка Q_PROPERTY к QWidget (если widgetProperty == nullptr, ищем USER-свойство)
    bool bind(QObject *source, const char *property, QWidget *widget, const char *widgetProperty = nullptr);

private:
    // Поиск свойства с флагом USER или с заданным именем
    QMetaProperty findTargetProperty(QObject *target, const QByteArray &preferredName) const;
};

#endif // PROPERTYWIDGETMAPPER_H
