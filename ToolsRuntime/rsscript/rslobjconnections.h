#ifndef RSLOBJCONNECTIONS_H
#define RSLOBJCONNECTIONS_H

#include <QObject>

class QSignalSpy;
class RslObjConnections : public QObject
{
    Q_OBJECT
public:
    RslObjConnections(QObject *parent = nullptr);
    virtual ~RslObjConnections();

    void setMacroFunc(const QString &macro);
    void initSignalSpy(const QObject *obj, const char *signal);
    int execIndex() const;

public slots:
    void ExecuteMacroFunc();

private:
    QString m_MacroName;
    QScopedPointer<QSignalSpy> m_pSpy;
};

#endif // RSLOBJCONNECTIONS_H
