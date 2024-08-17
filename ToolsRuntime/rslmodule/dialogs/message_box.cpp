#include "../stddialogs.h"
#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"
#include <limits>
#include <QApplication>
#include <QMessageBox>

void msgGetButtonConstantName()
{
    enum
    {
        prm_Button,
    };

    int btn = 0;
    QString result;
    if (GetFuncParamType(prm_Button) == QVariant::Int)
        btn = GetFuncParam(prm_Button).toInt();
    else
        ThrowParamTypeError(prm_Button);

    switch (btn) 
    {
        case QMessageBox::Ok:
            result = "MsgBtn_Ok";
            break;
        case QMessageBox::Open:
            result = "MsgBtn_Open";
            break;
        case QMessageBox::Save:
            result = "MsgBtn_Save";
            break;
        case QMessageBox::Cancel:
            result = "MsgBtn_Cancel";
            break;
        case QMessageBox::Close:
            result = "MsgBtn_Close";
            break;
        case QMessageBox::Discard:
            result = "MsgBtn_Discard";
            break;
        case QMessageBox::Apply:
            result = "MsgBtn_Apply";
            break;
        case QMessageBox::Reset:
            result = "MsgBtn_Reset";
            break;
        case QMessageBox::RestoreDefaults:
            result = "MsgBtn_RestoreDefaults";
            break;
        case QMessageBox::Help:
            result = "MsgBtn_Help";
            break;
        case QMessageBox::SaveAll:
            result = "MsgBtn_SaveAll";
            break;
        case QMessageBox::Yes:
            result = "MsgBtn_Yes";
            break;
        case QMessageBox::YesToAll:
            result = "MsgBtn_YesToAll";
            break;
        case QMessageBox::No:
            result = "MsgBtn_No";
            break;
        case QMessageBox::NoToAll:
            result = "MsgBtn_NoToAll";
            break;
        case QMessageBox::Abort:
            result = "MsgBtn_Abort";
            break;
        case QMessageBox::Retry:
            result = "MsgBtn_Retry";
            break;
        case QMessageBox::Ignore:
            result = "MsgBtn_Ignore";
            break;
        case QMessageBox::NoButton:
            result = "MsgBtn_NoButton";
            break;
        default:
            result = "Unknown constant";
    }

    SetReturnVal(result);
}

void msgCritical()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Buttons,
        prm_DefaultButton,
    };

    QString title, msg;
    int Buttons = QMessageBox::Ok;
    int DefaultButton = QMessageBox::NoButton;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Buttons) == QVariant::Int)
        Buttons = GetFuncParam(prm_Buttons).toInt();

    if (GetFuncParamType(prm_DefaultButton) == QVariant::Int)
        DefaultButton = GetFuncParam(prm_DefaultButton).toInt();

    QWidget *parent = GetTopLevelWidget();
    QMessageBox::StandardButton result = QMessageBox::critical(parent, title, msg, 
            (QMessageBox::StandardButtons)Buttons, 
            (QMessageBox::StandardButton)DefaultButton);

    SetReturnVal((int)result);
}

void msgWarning()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Buttons,
        prm_DefaultButton,
    };

    QString title, msg;
    int Buttons = QMessageBox::Ok;
    int DefaultButton = QMessageBox::NoButton;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Buttons) == QVariant::Int)
        Buttons = GetFuncParam(prm_Buttons).toInt();

    if (GetFuncParamType(prm_DefaultButton) == QVariant::Int)
        DefaultButton = GetFuncParam(prm_DefaultButton).toInt();

    QWidget *parent = GetTopLevelWidget();
    QMessageBox::StandardButton result = QMessageBox::warning(parent, title, msg,
        (QMessageBox::StandardButtons)Buttons,
        (QMessageBox::StandardButton)DefaultButton);

    SetReturnVal((int)result);
}

void msgQuestion()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Buttons,
        prm_DefaultButton,
    };

    QString title, msg;
    int Buttons = QMessageBox::Yes | QMessageBox::No;
    int DefaultButton = QMessageBox::NoButton;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Buttons) == QVariant::Int)
        Buttons = GetFuncParam(prm_Buttons).toInt();

    if (GetFuncParamType(prm_DefaultButton) == QVariant::Int)
        DefaultButton = GetFuncParam(prm_DefaultButton).toInt();

    QWidget *parent = GetTopLevelWidget();
    QMessageBox::StandardButton result = QMessageBox::question(parent, title, msg,
        (QMessageBox::StandardButtons)Buttons,
        (QMessageBox::StandardButton)DefaultButton);

    SetReturnVal((int)result);
}

void msgInformation()
{
    enum
    {
        prm_Title = 0,
        prm_Msg,
        prm_Buttons,
        prm_DefaultButton,
    };

    QString title, msg;
    int Buttons = QMessageBox::Ok;
    int DefaultButton = QMessageBox::NoButton;

    if (GetFuncParamType(prm_Title) == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    if (GetFuncParamType(prm_Msg) == QVariant::String)
        msg = GetFuncParam(prm_Msg).toString();

    if (GetFuncParamType(prm_Buttons) == QVariant::Int)
        Buttons = GetFuncParam(prm_Buttons).toInt();

    if (GetFuncParamType(prm_DefaultButton) == QVariant::Int)
        DefaultButton = GetFuncParam(prm_DefaultButton).toInt();

    QWidget *parent = GetTopLevelWidget();
    QMessageBox::StandardButton result = QMessageBox::information(parent, title, msg,
        (QMessageBox::StandardButtons)Buttons,
        (QMessageBox::StandardButton)DefaultButton);

    SetReturnVal((int)result);
}