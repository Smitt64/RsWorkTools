// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "codeeditor/codehighlighter.h"
#include "toolsruntinersl.h"
#include "errordlg.h"
#include "rslexecutor.h"
#include <errorsmodel.h>

extern void toolConnect();
Q_GLOBAL_STATIC(ToolsRuntime, pToolsRuntime)

/**
 * @brief Форматирует строку с использованием аргументов функции
 */
static void Rsl_toolFormatStr()
{
    /**
     * @brief Параметры функции
     */
    enum
    {
        prm_Format = 0,
    };

    /**
     * @brief Проверяет тип параметра формата
     */
    if (GetFuncParamType(prm_Format) != QVariant::String)
        ThrowParamTypeError(prm_Format);

    /**
     * @brief Получает формат из параметра функции
     */
    QString format = GetFuncParam(prm_Format).toString();

    /**
     * @brief Получает количество параметров функции
     */
    int size = GetFuncParamCount();
    /**
     * @brief Проходит по всем параметрам функции, начиная с первого аргумента формата
     */
    for (int i = 1; i < size; i++)
        /**
         * @brief Заменяет в формате все вхождения % на соответствующий аргумент функции
         */
        format = format.arg(GetFuncParam(i).toString());

    /**
     * @brief Устанавливает возвращаемое значение функции
     */
    SetReturnVal(format);
}


ToolsRuntimeModule::ToolsRuntimeModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<ToolsRuntime>();
    RegisterObjList::inst()->RegisterRslObject<ErrorsModel>();
    RegisterObjList::inst()->RegisterRslObject<ErrorDlg>(GenInfoUseParentProps | GenInfoUseParentMeths);
}

void ToolsRuntimeModule::Init()
{

}

void ToolsRuntimeModule::Proc()
{
    addConstant("ErrorModeInformation", ErrorDlg::ModeInformation);
    addConstant("ErrorModeMessageBox", ErrorDlg::ModeMessageBox);
    addConstant("ErrorModeWidget", ErrorDlg::ModeWidget);

    addConstant("HighlighterPlain", HighlighterPlain);
    addConstant("HighlighterCpp", HighlighterCpp);
    addConstant("HighlighterRsl", HighlighterRsl);
    addConstant("HighlighterSql", HighlighterSql);
    addConstant("HighlighterXml", HighlighterXml);

    addConstant("ToolsRuntime", QVariant::fromValue((QObject*)pToolsRuntime));

    RegisterObjList::inst()->AddObject<ErrorsModel>();
    RegisterObjList::inst()->AddObject<ErrorDlg>();
    RegisterObjList::inst()->AddObject<ToolsRuntime>(false);
    RegisterObjList::inst()->AddStdProc("toolFormatStr", Rsl_toolFormatStr);
    RegisterObjList::inst()->AddStdProc("toolConnect", toolConnect);
}
