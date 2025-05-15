// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern const char *DefaultStyleName;
class ChildObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message);
public:

    Q_INVOKABLE ChildObject() :
        QObject()
    {
        _message = "This is ChildObject";
    }

    Q_INVOKABLE ChildObject(const ChildObject &other)
    {

    }

    QString message()
    {
        return _message;
    }

private:
    QString _message;
};

Q_DECLARE_TYPEINFO(ChildObject, Q_COMPLEX_TYPE);
Q_DECLARE_OPAQUE_POINTER(ChildObject);

class TestObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage);
    Q_PROPERTY(QStringList stringlist READ stringlist);
    Q_PROPERTY(QVariantList varlist READ varlist);
    Q_PROPERTY(ChildObject* child READ childObj);
public:
    Q_INVOKABLE TestObject() :
        QObject()
    {
        _child = new ChildObject();
        _message = "Empty Message";
        _StringList = QStringList{ "один", "два", "три" };
    }

    Q_INVOKABLE int testMethod(int k = 0)
    {
        return k;
    }

    Q_INVOKABLE int testMethod(QString k)
    {
        return 0;
    }

    QVariantList varlist()
    {
        return _VariantList;
    }

    Q_INVOKABLE QVariant testVariant(const QVariant &var)
    {
        return QVariant("var: " + var.toString());
    }

    Q_INVOKABLE QRect testRect(const QRect &var)
    {
        QRect rc = var;
        rc.moveTo(10, 10);
        return rc;
    }

    Q_INVOKABLE ChildObject *childObj()
    {
        return _child;
    }

    Q_INVOKABLE void passObj(ChildObject *obj)
    {
        qDebug() << "passObj" << obj;
    }

    Q_INVOKABLE QObject *nullObj()
    {
        return nullptr;
    }

    QString message()
    {
        return _message;
    }

    void setMessage(QString msg)
    {
        _message = msg;
    }

    QStringList stringlist()
    {
        return _StringList;
    }

    Q_INVOKABLE void setStringlist(QStringList lst)
    {
        _StringList = lst;
    }

    Q_INVOKABLE void setVariantlist(QVariantList lst)
    {
        _VariantList = lst;
    }

signals:
    void testSignal(int i);

private:
    ChildObject *_child;
    QString _message;
    QStringList _StringList;
    QVariantList _VariantList;
};

class CodeEditor;
class QSignalSpy;
class QCloseEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();
    void testSlot();

private:
    Ui::MainWindow *ui;
    TestObject *obj;
    QSignalSpy *spy;
    CodeEditor *pEditor, *pOutput;
    QTabWidget *pContainer;
    QStringListModel m_Errors;
};

#endif // MAINWINDOW_H
