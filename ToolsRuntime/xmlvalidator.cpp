#include "xmlvalidator.h"
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QFile>
#include <QAbstractMessageHandler>
#include "errorsmodel.h"

class XmlValidatorErrorHandler : public QAbstractMessageHandler
{
public:
    explicit XmlValidatorErrorHandler(QObject* parent = nullptr) :
        QAbstractMessageHandler(parent),
        m_errorModel(nullptr),
        m_hasErrors(false)
    {

    }

    ~XmlValidatorErrorHandler()
    {

    }

    void setErrorModel(ErrorsModel* errorModel)
    {
        m_errorModel = errorModel;
        clear();
    }

    ErrorsModel* errorModel() const
    {
        return m_errorModel;
    }

    QString lastError() const
    {
        return m_lastError;
    }

    bool hasErrors() const
    {
        return m_hasErrors;
    }

    void clear()
    {
        m_lastError.clear();
        m_hasErrors = false;
    }

    void handleMessage(QtMsgType type,
                                                 const QString& description,
                                                 const QUrl& identifier,
                                                 const QSourceLocation& sourceLocation)
    {
        Q_UNUSED(identifier);

        QString location = tr(" в строке %1, столбец %2,")
                               .arg(sourceLocation.line()).arg(sourceLocation.column());

        QString msg = tr("XSD message:");

        if (!location.isEmpty())
            msg += location;

        msg += " " + description;

        if (m_errorModel)
        {
        switch (type)
        {
            case QtDebugMsg:
            case QtInfoMsg:
                m_errorModel->addMessage(msg);
                break;
            case QtWarningMsg:
                m_errorModel->appendError(msg, ErrorsModel::TypeWarning);
                break;
            case QtCriticalMsg:
            case QtFatalMsg:
                m_errorModel->appendError(msg);
                break;
            default:
                break;
            }
        }

        m_lastError = msg;
        m_hasErrors = true;
    }

private:
    ErrorsModel* m_errorModel;
    QString m_lastError;
    bool m_hasErrors;
};

class XmlValidatorPrivate
{
    Q_DECLARE_PUBLIC(XmlValidator)

public:
    explicit XmlValidatorPrivate(XmlValidator* q) :
        q_ptr(q),
        errorHandler(new XmlValidatorErrorHandler(q)),
        isValid(false),
        schemaLoaded(false)
    {

    }

    ~XmlValidatorPrivate()
    {
        cleanup();
    }

    bool loadSchema()
    {
        Q_Q(XmlValidator);

        if (schemaLoaded)
            return schema.isValid();

        QFile xsdFile(":/res/reslib.xsd");
        if (!xsdFile.open(QIODevice::ReadOnly))
        {
            lastError = QString("Cannot open XSD schema: %1").arg(xsdFile.errorString());
            return false;
        }

        if (!schema.load(&xsdFile, QUrl::fromLocalFile(xsdFile.fileName())))
        {
            lastError = "Failed to load XSD schema: Unknown schema error";
            return false;
        }

        schemaLoaded = true;

        if (!schema.isValid())
        {
            lastError = "XSD schema is invalid";
            return false;
        }

        return true;
    }

    bool validateInternal(QIODevice* xmlDevice, ErrorsModel* errorModel)
    {
        QXmlSchemaValidator validator(schema);
        errorHandler->setErrorModel(errorModel);
        validator.setMessageHandler(errorHandler);

        xmlDevice->seek(0);

        if (!validator.validate(xmlDevice, QUrl::fromLocalFile("input.xml")))
        {
            lastError = errorHandler->lastError();
            if (lastError.isEmpty())
                lastError = "XML validation failed against XSD schema";

            return false;
        }

        return true;
    }

    void cleanup()
    {
        schemaLoaded = false;
        isValid = false;
        lastError.clear();
    }

    XmlValidator* q_ptr;

    QXmlSchema schema;
    XmlValidatorErrorHandler* errorHandler;
    QString lastError;
    bool isValid;
    bool schemaLoaded;
};

XmlValidator::XmlValidator(QObject *parent)
    : QObject{parent},
    d_ptr(new XmlValidatorPrivate(this))
{

}

XmlValidator::~XmlValidator()
{

}

bool XmlValidator::validateXmlWithXsd(QIODevice* xmlDevice, ErrorsModel* errorModel)
{
    Q_D(XmlValidator);

    emit validationStarted();
    d->lastError.clear();

    if (!xmlDevice) {
        d->lastError = "Invalid XML device";
        emit validationFinished(false);
        return false;
    }

    if (!d->loadSchema()) {
        if (errorModel) {
            errorModel->addError(d->lastError);
        }
        emit validationFinished(false);
        return false;
    }

    bool result = d->validateInternal(xmlDevice, errorModel);

    d->isValid = result;
    emit validationFinished(result);

    return result;
}

bool XmlValidator::validateXmlWithXsd(const QString& xmlFilePath, ErrorsModel* errorModel)
{
    QFile xmlFile(xmlFilePath);
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        QString error = QString("Cannot open XML file: %1").arg(xmlFile.errorString());
        if (errorModel) {
            errorModel->addError(error);
        }
        emit validationFinished(false);
        return false;
    }

    return validateXmlWithXsd(&xmlFile, errorModel);
}

QString XmlValidator::lastError() const
{
    Q_D(const XmlValidator);
    return d->lastError;
}

bool XmlValidator::isValid() const
{
    Q_D(const XmlValidator);
    return d->isValid;
}

bool XmlValidator::validate(QIODevice* xmlDevice, ErrorsModel* errorModel)
{
    XmlValidator validator;
    return validator.validateXmlWithXsd(xmlDevice, errorModel);
}

bool XmlValidator::validate(const QString& xmlFilePath, ErrorsModel* errorModel)
{
    XmlValidator validator;
    return validator.validateXmlWithXsd(xmlFilePath, errorModel);
}

void XmlValidator::cancelValidation()
{
    Q_D(XmlValidator);
    d->lastError = "Validation canceled";
    emit validationFinished(false);
}
