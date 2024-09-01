#include "rslobjconnections.h"
#include "rslexecutor.h"
#include <QDebug>
#include <QSignalSpy>

RslObjConnections::RslObjConnections(QObject *parent)
    : QObject{parent}
{

}

RslObjConnections::~RslObjConnections()
{

}

void RslObjConnections::setMacroFunc(const QString &macro)
{
    m_MacroName = macro;
}

int RslObjConnections::execIndex() const
{
    const QMetaObject *meta = metaObject();
    return meta->indexOfMethod("ExecuteMacroFunc()");
}

void RslObjConnections::initSignalSpy(const QObject *obj, const char *signal)
{
    if (signal[0] == '2')
        m_pSpy.reset(new QSignalSpy(obj, signal));
    else
    {
        QString method = QString("2%1").arg(signal);
        m_pSpy.reset(new QSignalSpy(obj, method.toLocal8Bit().data()));
    }
}

void RslObjConnections::ExecuteMacroFunc()
{
    RslExecutor *excutor = qobject_cast<RslExecutor*>(parent());
    QList<QVariant> arguments = m_pSpy->takeFirst();

    if (!m_MacroName.isEmpty())
        excutor->call(m_MacroName, arguments);
}
