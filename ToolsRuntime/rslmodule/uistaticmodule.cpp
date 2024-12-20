#include "uistaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"

#include <QAbstractButton>
#include <QAbstractSlider>
#include <QAbstractSpinBox>
#include <QCalendarWidget>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFocusFrame>
#include <QFrame>
#include <QGroupBox>
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QOpenGLWidget>
#include <QProgressBar>
#include <QSplashScreen>
#include <QSplitterHandle>
#include <QStatusBar>
#include <QTabBar>
#include <QTabWidget>
#include <QToolBar>
#include <QWizardPage>
#include <QPushButton>
#include <QPlainTextEdit>

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QUiLoader>
#include <QDebug>

static void Rsl_uiLoadFile()
{
    enum
    {
        prm_filename = 0,
    };

    if (GetFuncParamType(prm_filename) != QVariant::String)
        ThrowParamTypeError(prm_filename);

    QWidget *parent = qApp->activeWindow();

    QStringList lst = RegisterObjList::inst()->incDirs().split(";");

    QString realFileName;
    QString filename = GetFuncParam(prm_filename).toString();

    QFileInfo fi(filename);
    if (fi.dir() != QDir("."))
        realFileName = filename;
    else
    {
        for (const QString &item : lst)
        {
            QDir d(item);
            if (QFile::exists(d.absoluteFilePath(filename)))
            {
                realFileName = d.absoluteFilePath(filename);
                break;
            }
        }
    }

    QWidget *widget = nullptr;
    if (!realFileName.isEmpty())
    {
        QFile f(realFileName);

        if (f.open(QFile::ReadOnly))
        {
            QUiLoader loader;
            widget = loader.load(&f, parent);
            f.close();
        }
    }

    SetReturnVal(QVariant::fromValue<QObject*>(widget));
}

static void Rsl_uiFindChild()
{
    enum
    {
        prm_parent = 0,
        prm_name,
        prm_DirectChildrenOnly
    };

    if (GetFuncParamType(prm_parent) != QVariant::UserType)
        ThrowParamTypeError(prm_parent);

    if (GetFuncParamType(prm_name) != QVariant::String)
        ThrowParamTypeError(prm_name);

    bool DirectChildrenOnly = false;
    if (GetFuncParamType(prm_DirectChildrenOnly) == QVariant::Bool)
        DirectChildrenOnly = GetFuncParam(prm_DirectChildrenOnly).toBool();

    QWidget *widget = nullptr;
    QWidget *parent = GetFuncParam<QWidget*>(prm_parent);
    QString name = GetFuncParam(prm_name).toString();
    Qt::FindChildOptions options = Qt::FindChildrenRecursively;

    if (DirectChildrenOnly)
        options = Qt::FindDirectChildrenOnly;

    widget = parent->findChild<QWidget*>(name, options);

    SetReturnVal(QVariant::fromValue<QObject*>(widget));
}

static void Rsl_uiGetDialogButton()
{
    enum
    {
        prm_buttonbox = 0,
        prm_button
    };

    QDialogButtonBox *box = GetFuncParam<QDialogButtonBox*>(prm_buttonbox);

    if (!box)
        ThrowParamTypeError<QDialogButtonBox>(prm_buttonbox);

    if (GetFuncParamType(prm_button) != QVariant::Int)
        ThrowParamTypeError(prm_button);

    QPushButton *btn = box->button((QDialogButtonBox::StandardButton)GetFuncParam(prm_button).toInt());

    if (btn)
        SetReturnVal(QVariant::fromValue<QObject*>(btn));
}

static void Rsl_uiDialogButtonAdd()
{
    enum
    {
        prm_buttonbox = 0,
        prm_buttontext,
        prm_buttonrole
    };

    QDialogButtonBox *box = GetFuncParam<QDialogButtonBox*>(prm_buttonbox);

    if (!box)
        ThrowParamTypeError<QDialogButtonBox>(prm_buttonbox);

    if (GetFuncParamType(prm_buttontext) != QVariant::String)
        ThrowParamTypeError(prm_buttontext);

    if (GetFuncParamType(prm_buttonrole) != QVariant::Int)
        ThrowParamTypeError(prm_buttonrole);

    int role = GetFuncParam(prm_buttonrole).toInt();
    QPushButton *btn = box->addButton(GetFuncParam(prm_buttontext).toString(), (QDialogButtonBox::ButtonRole)role);
    SetReturnVal(QVariant::fromValue<QObject*>(btn));
}

static void Rsl_uiTextEditAppend()
{
    enum
    {
        prm_editor = 0,
        prm_text,
    };

    if (GetFuncParamType(prm_text) != QVariant::String)
        ThrowParamTypeError(prm_text);

    QPlainTextEdit *plainText = GetFuncParam<QPlainTextEdit*>(prm_editor);

    if (plainText)
    {
        plainText->moveCursor (QTextCursor::End);
        plainText->insertPlainText(GetFuncParam(prm_text).toString());
        plainText->moveCursor (QTextCursor::End);
    }
    else
    {
        QTextEdit *edit = GetFuncParam<QTextEdit*>(prm_editor);

        if (edit)
        {
            plainText->moveCursor (QTextCursor::End);
            plainText->insertPlainText(GetFuncParam(prm_text).toString());
            plainText->moveCursor (QTextCursor::End);
        }
        else
            ThrowParamTypeError(prm_editor);
    }
}

UiStaticModule::UiStaticModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<QPushButton>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QAbstractButton>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QAbstractSlider>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QAbstractSpinBox>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QCalendarWidget>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QComboBox>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QDialog>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QDialogButtonBox>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QDockWidget>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QFocusFrame>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QFrame>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QGroupBox>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QKeySequenceEdit>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QLineEdit>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QMainWindow>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QMdiSubWindow>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QMenu>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QMenuBar>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QOpenGLWidget>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QProgressBar>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QSplashScreen>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QSplitterHandle>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QStatusBar>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QTabBar>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QTabWidget>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QToolBar>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QWizardPage>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<QPlainTextEdit>(GenInfoUseParentProps | GenInfoUseParentMeths);
}

void UiStaticModule::Init()
{

}

void UiStaticModule::Proc()
{
    addConstant("ButtonInvalidRole", QDialogButtonBox::InvalidRole);
    addConstant("ButtonAcceptRole", QDialogButtonBox::AcceptRole);
    addConstant("ButtonRejectRole", QDialogButtonBox::RejectRole);
    addConstant("ButtonDestructiveRole", QDialogButtonBox::DestructiveRole);
    addConstant("ButtonActionRole", QDialogButtonBox::ActionRole);
    addConstant("ButtonHelpRole", QDialogButtonBox::HelpRole);
    addConstant("ButtonYesRole", QDialogButtonBox::YesRole);
    addConstant("ButtonNoRole", QDialogButtonBox::NoRole);
    addConstant("ButtonResetRole", QDialogButtonBox::ResetRole);
    addConstant("ButtonApplyRole", QDialogButtonBox::ApplyRole);

    RegisterObjList::inst()->AddStdProc("uiLoadFile", Rsl_uiLoadFile);
    RegisterObjList::inst()->AddStdProc("uiFindChild", Rsl_uiFindChild);
    RegisterObjList::inst()->AddStdProc("uiGetDialogButton", Rsl_uiGetDialogButton);
    RegisterObjList::inst()->AddStdProc("uiDialogButtonAdd", Rsl_uiDialogButtonAdd);
    RegisterObjList::inst()->AddStdProc("uiTextEditAppend", Rsl_uiTextEditAppend);

    RegisterObjList::inst()->AddObject<QPushButton>(false);
    RegisterObjList::inst()->AddObject<QAbstractButton>(false);
    RegisterObjList::inst()->AddObject<QAbstractSlider>(false);
    RegisterObjList::inst()->AddObject<QAbstractSpinBox>(false);
    RegisterObjList::inst()->AddObject<QCalendarWidget>(false);
    RegisterObjList::inst()->AddObject<QComboBox>(false);
    RegisterObjList::inst()->AddObject<QDialog>(false);
    RegisterObjList::inst()->AddObject<QDialogButtonBox>(false);
    RegisterObjList::inst()->AddObject<QDockWidget>(false);
    RegisterObjList::inst()->AddObject<QFocusFrame>(false);
    RegisterObjList::inst()->AddObject<QFrame>(false);
    RegisterObjList::inst()->AddObject<QGroupBox>(false);
    RegisterObjList::inst()->AddObject<QKeySequenceEdit>(false);
    RegisterObjList::inst()->AddObject<QLineEdit>(false);
    RegisterObjList::inst()->AddObject<QMainWindow>(false);
    RegisterObjList::inst()->AddObject<QMdiSubWindow>(false);
    RegisterObjList::inst()->AddObject<QMenu>(false);
    RegisterObjList::inst()->AddObject<QMenuBar>(false);
    RegisterObjList::inst()->AddObject<QOpenGLWidget>(false);
    RegisterObjList::inst()->AddObject<QProgressBar>(false);
    RegisterObjList::inst()->AddObject<QSplashScreen>(false);
    RegisterObjList::inst()->AddObject<QSplitterHandle>(false);
    RegisterObjList::inst()->AddObject<QStatusBar>(false);
    RegisterObjList::inst()->AddObject<QTabBar>(false);
    RegisterObjList::inst()->AddObject<QTabWidget>(false);
    RegisterObjList::inst()->AddObject<QToolBar>(false);
    RegisterObjList::inst()->AddObject<QWizardPage>(false);
    RegisterObjList::inst()->AddObject<QPlainTextEdit>(false);
}
