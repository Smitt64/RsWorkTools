#include "stddialogs.h"
#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"
#include <limits>
#include <QInputDialog>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

QWidget* GetTopLevelWidget()
{
    return QApplication::activeWindow();
}

static void toolGetText()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Default
    };

    QString title, msg, def;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Default) == QVariant::String)
        def = GetFuncParam(prm_Default).toString();

    QWidget *parent = GetTopLevelWidget();

    QString query = QInputDialog::getText(parent, title, msg, QLineEdit::Normal, def);
    SetReturnVal(query);
}

static void toolGetNumber()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Default,
        prm_Min,
        prm_Max
    };

    QString title, msg;
    int min = std::numeric_limits<int>::min(), max = std::numeric_limits<int>::max(), def;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Default) == QVariant::Int)
        def = GetFuncParam(prm_Default).toInt();
    
    if (GetFuncParamType(prm_Min) == QVariant::Int)
        min = GetFuncParam(prm_Min).toInt();
    
    if (GetFuncParamType(prm_Max) == QVariant::Int)
        max = GetFuncParam(prm_Max).toInt();
    
    QWidget *parent = GetTopLevelWidget();
    int val = QInputDialog::getInt(parent, title, msg, def, min, max);
    SetReturnVal(val);
}

static void toolGetItem()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_List,
        prm_Current,
        prm_Editable
    };

    QString title, msg;
    QStringList list;
    int current = 0;
    bool editable = false;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_List) == QVariant::StringList)
        list = GetFuncParam(prm_List, true).toStringList();
    else
        ThrowParamTypeError(prm_List);

    if (GetFuncParamType(prm_Current) == QVariant::Int)
        current = GetFuncParam(prm_Current).toInt();

    if (GetFuncParamType(prm_Editable) == QVariant::Bool)
        editable = GetFuncParam(prm_Editable).toBool();

    QWidget *parent = GetTopLevelWidget();
    QString val = QInputDialog::getItem(parent, title, msg, list, current, editable);
    SetReturnVal(val);
}

static void toolGetMultyLineText()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Default
    };

    QString title, msg, def;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Default) == QVariant::String)
        def = GetFuncParam(prm_Default).toString();

    QWidget *parent = GetTopLevelWidget();
    QString query = QInputDialog::getMultiLineText(parent, title, msg, def);
    SetReturnVal(query);
}

static void toolGetDouble()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Default,
        prm_Min,
        prm_Max,
        prm_Decimals
    };

    QString title, msg;
    double min = std::numeric_limits<double>::min(),
        max = std::numeric_limits<double>::max(), def = 0.0;
    int decimals = 2;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Default) == QVariant::Double || GetFuncParamType(prm_Default) == QVariant::Int)
        def = GetFuncParam(prm_Default).toDouble();
    
    if (GetFuncParamType(prm_Min) == QVariant::Double)
        min = GetFuncParam(prm_Min).toDouble();
    
    if (GetFuncParamType(prm_Max) == QVariant::Double)
        max = GetFuncParam(prm_Max).toDouble();

    if (GetFuncParamType(prm_Decimals) == QVariant::Int)
        decimals = GetFuncParam(prm_Decimals).toInt();

    QWidget *parent = GetTopLevelWidget();
    double val = QInputDialog::getDouble(parent, title, msg, def, min, max, decimals);
    SetReturnVal(val);
}

static void toolGetMulyLineText()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Default
    };

    QString title, msg, def;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Default) == QVariant::String)
        def = GetFuncParam(prm_Default).toString();

    QWidget *parent = GetTopLevelWidget();
    QString query = QInputDialog::getMultiLineText(parent, title, msg, def);
    SetReturnVal(query);
}

static void toolExistingDirectory()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Dir
    };

    QString title, msg, dir;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Dir) == QVariant::String)
        dir = GetFuncParam(prm_Dir).toString();

    QWidget *parent = GetTopLevelWidget();
    QString val = QFileDialog::getExistingDirectory(parent, title, dir);
    SetReturnVal(val);
}

static void toolOpenFileName()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Dir,
        prm_Filter
    };

    QString title, msg, dir, filter;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Dir) == QVariant::String)
        dir = GetFuncParam(prm_Dir).toString();

    if (GetFuncParamType(prm_Filter) == QVariant::String)
        filter = GetFuncParam(prm_Filter).toString();

    QWidget *parent = GetTopLevelWidget();
    QString val = QFileDialog::getOpenFileName(parent, title, dir, filter);
    SetReturnVal(val);
}

static void toolSaveFileName()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Dir,
        prm_Filter
    };

    QString title, msg, dir, filter;
    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Dir) == QVariant::String)
        dir = GetFuncParam(prm_Dir).toString();

    if (GetFuncParamType(prm_Filter) == QVariant::String)
        filter = GetFuncParam(prm_Filter).toString();

    QWidget *parent = GetTopLevelWidget();
    QString val = QFileDialog::getSaveFileName(parent, title, dir, filter);
    SetReturnVal(val);
}

StdDialogs::StdDialogs() :
    RslStaticModule()
{

}

void StdDialogs::Init() 
{

}

void StdDialogs::Proc() 
{
    addConstant("MsgBtn_Ok", QMessageBox::Ok);
    addConstant("MsgBtn_Open", QMessageBox::Open);
    addConstant("MsgBtn_Save", QMessageBox::Save);
    addConstant("MsgBtn_Cancel", QMessageBox::Cancel);
    addConstant("MsgBtn_Close", QMessageBox::Close);
    addConstant("MsgBtn_Discard", QMessageBox::Discard);
    addConstant("MsgBtn_Apply", QMessageBox::Apply);
    addConstant("MsgBtn_Reset", QMessageBox::Reset);
    addConstant("MsgBtn_RestoreDefaults", QMessageBox::RestoreDefaults);
    addConstant("MsgBtn_Help", QMessageBox::Help);
    addConstant("MsgBtn_SaveAll", QMessageBox::SaveAll);
    addConstant("MsgBtn_Yes", QMessageBox::Yes);
    addConstant("MsgBtn_YesToAll", QMessageBox::YesToAll);
    addConstant("MsgBtn_No", QMessageBox::No);
    addConstant("MsgBtn_NoToAll", QMessageBox::NoToAll);
    addConstant("MsgBtn_Abort", QMessageBox::Abort);
    addConstant("MsgBtn_Retry", QMessageBox::Retry);
    addConstant("MsgBtn_Ignore", QMessageBox::Ignore);
    addConstant("MsgBtn_NoButton", QMessageBox::NoButton);

    RegisterObjList::inst()->AddStdProc("toolGetText", toolGetText);
    RegisterObjList::inst()->AddStdProc("toolGetNumber", toolGetNumber);
    RegisterObjList::inst()->AddStdProc("toolGetItem", toolGetItem);
    RegisterObjList::inst()->AddStdProc("toolGetMultyLineText", toolGetMultyLineText);
    RegisterObjList::inst()->AddStdProc("toolGetDouble", toolGetDouble);
    RegisterObjList::inst()->AddStdProc("toolExistingDirectory", toolExistingDirectory);
    RegisterObjList::inst()->AddStdProc("toolOpenFileName", toolOpenFileName);
    RegisterObjList::inst()->AddStdProc("toolSaveFileName", toolSaveFileName);
    RegisterObjList::inst()->AddStdProc("toolGetMulyLineText", toolGetMulyLineText);

    RegisterObjList::inst()->AddStdProc("msgCritical", msgCritical);
    RegisterObjList::inst()->AddStdProc("msgWarning", msgWarning);
    RegisterObjList::inst()->AddStdProc("msgInformation", msgInformation);
    RegisterObjList::inst()->AddStdProc("msgQuestion", msgQuestion);
    RegisterObjList::inst()->AddStdProc("msgGetButtonConstantName", msgGetButtonConstantName);
}
