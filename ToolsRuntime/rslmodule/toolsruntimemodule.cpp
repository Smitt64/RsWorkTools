// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "codeeditor/codehighlighter.h"
#include "toolsruntinersl.h"
#include "toolsruntime.h"
#include "rsscript/rslibdynamicfuncs.h"
#include "errordlg.h"
#include "rslexecutor.h"
#include "spelling/spellchecker.h"
#include "spelling/spellcheckerdlg.h"
#include "rslmodule/iterableobjectbase.h"
#include "rslmodule/rslstringlist.h"
#include "rslmodule/variantlist.h"
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

static void Rsl_spellGetDictionaryPath()
{
    SetReturnVal(spellGetDictionaryPath());
}

static void Rsl_spellGetCheckerForLanguage()
{
    enum
    {
        prm_language = 0,
    };

    SpellChecker *checker = nullptr;
    if (GetFuncParamType(prm_language) != QVariant::String)
        ThrowParamTypeError<QString>(prm_language);
    
    QString language = GetFuncParam(prm_language).toString();
    spellGetCheckerForLanguage(language, &checker);

    if (checker)
        SetReturnVal(QVariant::fromValue<QObject*>(checker));
}

ToolsRuntimeModule::ToolsRuntimeModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<ToolsRuntime>();
    RegisterObjList::inst()->RegisterRslObject<ErrorsModel>();
    RegisterObjList::inst()->RegisterRslObject<SpellChecker>();
    RegisterObjList::inst()->RegisterRslObject<SpellCheckerDlg>();
    RegisterObjList::inst()->RegisterRslObject<ErrorDlg>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IterableObjectBase>();

    RegisterObjList::inst()->RegisterRslObject<StringList>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<VariantList>(GenInfoUseParentProps | GenInfoUseParentMeths);
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

    addConstant("SpellAbortCheck", SpellCheckerDlg::AbortCheck);
    addConstant("SpellAddToDict", SpellCheckerDlg::AddToDict);
    addConstant("SpellIgnoreOnce", SpellCheckerDlg::IgnoreOnce);
    addConstant("SpellIgnoreAll", SpellCheckerDlg::IgnoreAll);
    addConstant("SpellReplaceOnce", SpellCheckerDlg::ReplaceOnce);
    addConstant("SpellReplaceAll", SpellCheckerDlg::ReplaceAll);

    addConstant("Base64Encoding", QByteArray::Base64Encoding);
    addConstant("Base64UrlEncoding", QByteArray::Base64UrlEncoding);
    addConstant("KeepTrailingEquals", QByteArray::KeepTrailingEquals);
    addConstant("OmitTrailingEquals", QByteArray::OmitTrailingEquals);
    addConstant("IgnoreBase64DecodingErrors", QByteArray::IgnoreBase64DecodingErrors);
    addConstant("AbortOnBase64DecodingErrors", QByteArray::AbortOnBase64DecodingErrors);

    addConstant("JavaRuntimeHomes32", JavaRuntimeHomes32);
    addConstant("JavaDevelopmentHomes32", JavaDevelopmentHomes32);
    addConstant("JavaRuntimeHomes64", JavaRuntimeHomes64);
    addConstant("JavaDevelopmentHomes64", JavaDevelopmentHomes64);
    addConstant("JavaAllHomes", JavaAllHomes);

    addConstant("Unchecked", Qt::Unchecked);
    addConstant("PartiallyChecked", Qt::PartiallyChecked);
    addConstant("Checked", Qt::Checked);

    addConstant("ToolsRuntime", QVariant::fromValue((QObject*)pToolsRuntime));

    RegisterObjList::inst()->AddObject<SpellChecker>();
    RegisterObjList::inst()->AddObject<SpellCheckerDlg>();
    RegisterObjList::inst()->AddObject<ErrorsModel>();
    RegisterObjList::inst()->AddObject<ErrorDlg>();
    RegisterObjList::inst()->AddObject<ToolsRuntime>(false);
    RegisterObjList::inst()->AddObject<StringList>();
    RegisterObjList::inst()->AddObject<VariantList>();
    
    RegisterObjList::inst()->AddStdProc("toolFormatStr", Rsl_toolFormatStr);
    RegisterObjList::inst()->AddStdProc("toolConnect", toolConnect);
    RegisterObjList::inst()->AddStdProc("spellGetDictionaryPath", Rsl_spellGetDictionaryPath);
    RegisterObjList::inst()->AddStdProc("spellGetCheckerForLanguage", Rsl_spellGetCheckerForLanguage);

    RegisterRectRsl();
    RegisterSizeRsl();
    RegisterPointRsl();
    RegisterByteArrayRsl();
}
