#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QObject>
#include <QIODevice>
#include <QScopedPointer>
#include "ToolsRuntime_global.h"

class ErrorsModel;
class XmlValidatorPrivate;
class TOOLSRUNTIME_EXPORT XmlValidator : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(XmlValidator)
    Q_DISABLE_COPY(XmlValidator)

public:
    explicit XmlValidator(QObject *parent = nullptr);
    virtual ~XmlValidator();

    bool validateXmlWithXsd(QIODevice* xmlDevice, ErrorsModel* errorModel = nullptr);
    bool validateXmlWithXsd(const QString& xmlFilePath, ErrorsModel* errorModel = nullptr);

    QString lastError() const;
    bool isValid() const;

    // Статические методы для удобства
    static bool validate(QIODevice* xmlDevice, ErrorsModel* errorModel = nullptr);
    static bool validate(const QString& xmlFilePath, ErrorsModel* errorModel = nullptr);

signals:
    void validationStarted();
    void validationFinished(bool isValid);
    void errorOccurred(const QString& error);

public slots:
    void cancelValidation();

protected:
    QScopedPointer<XmlValidatorPrivate> d_ptr;
};

#endif // XMLVALIDATOR_H
