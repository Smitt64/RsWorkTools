// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

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

    QString message()
    {
        return _message;
    }

private:
    QString _message;
};

class TestObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage);
    Q_PROPERTY(QStringList stringlist READ stringlist);
    Q_PROPERTY(ChildObject* child READ child);
public:
    Q_INVOKABLE TestObject() :
        QObject()
    {
        _child = new ChildObject();
        _message = "empty message";
        _StringList = QStringList{ "один", "два", "три" };
    }

    Q_INVOKABLE int testMethod(int k = 0)
    {
        return k;
    }

    Q_INVOKABLE QVariant testVariant(const QVariant &var)
    {
        return QVariant("var: " + var.toString());
    }

    ChildObject *child()
    {
        return _child;
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

    void setStringlist(QStringList lst)
    {
        _StringList = lst;
    }

private:
    ChildObject *_child;
    QString _message;
    QStringList _StringList;
};

class CodeEditor;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    CodeEditor *pEditor, *pOutput;
    QStringListModel m_Errors;
};
#endif // MAINWINDOW_H
