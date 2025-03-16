#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"
#include "strlib.h"

void strFill()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    int size = GetFuncParam(1).toInt();

    if (!str.isEmpty())
        SetReturnVal(QString(size, str[0]));
    else
        SetReturnVal(QString());
}

/**
 * @brief Преобразует строку в верхний регистр.
 * @param str Исходная строка.
 * @return Строка в верхнем регистре.
 * @see QString::toUpper()
 */
void strToUpper()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.toUpper());
}

/**
 * @brief Преобразует строку в нижний регистр.
 * @param str Исходная строка.
 * @return Строка в нижнем регистре.
 * @see QString::toLower()
 */
void strToLower()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.toLower());
}

/**
 * @brief Удаляет пробелы с начала и конца строки.
 * @param str Исходная строка.
 * @return Строка без пробелов по краям.
 * @see QString::trimmed()
 */
void strTrimmed()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.trimmed());
}

/**
 * @brief Удаляет лишние пробелы и заменяет их на один.
 * @param str Исходная строка.
 * @return Упрощенная строка.
 * @see QString::simplified()
 */
void strSimplified()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.simplified());
}

/**
 * @brief Проверяет, начинается ли строка с указанной подстроки.
 * @param str Исходная строка.
 * @param prefix Подстрока для проверки.
 * @return true, если строка начинается с подстроки, иначе false.
 * @see QString::startsWith()
 */
void strStartsWith()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString prefix = GetFuncParam(1).toString();
    SetReturnVal(str.startsWith(prefix));
}

/**
 * @brief Проверяет, заканчивается ли строка указанной подстроки.
 * @param str Исходная строка.
 * @param suffix Подстрока для проверки.
 * @return true, если строка заканчивается подстрокой, иначе false.
 * @see QString::endsWith()
 */
void strEndsWith()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString suffix = GetFuncParam(1).toString();
    SetReturnVal(str.endsWith(suffix));
}

/**
 * @brief Проверяет, содержит ли строка указанную подстроку.
 * @param str Исходная строка.
 * @param substring Подстрока для поиска.
 * @return true, если подстрока найдена, иначе false.
 * @see QString::contains()
 */
void strContains()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString substring = GetFuncParam(1).toString();
    SetReturnVal(str.contains(substring));
}

/**
 * @brief Возвращает индекс первого вхождения подстроки.
 * @param str Исходная строка.
 * @param substring Подстрока для поиска.
 * @param from Позиция, с которой начинать поиск (по умолчанию 0).
 * @return Индекс первого вхождения или -1, если подстрока не найдена.
 * @see QString::indexOf()
 */
void strIndexOf()
{
    int paramCount = GetFuncParamCount();
    if (paramCount < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString substring = GetFuncParam(1).toString();
    int from = (paramCount > 2) ? GetFuncParam(2).toInt() : 0;

    SetReturnVal(str.indexOf(substring, from));
}

/**
 * @brief Возвращает индекс последнего вхождения подстроки.
 * @param str Исходная строка.
 * @param substring Подстрока для поиска.
 * @param from Позиция, с которой начинать поиск (по умолчанию 0).
 * @return Индекс последнего вхождения или -1, если подстрока не найдена.
 * @see QString::lastIndexOf()
 */
void strLastIndexOf()
{
    int paramCount = GetFuncParamCount();
    if (paramCount < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString substring = GetFuncParam(1).toString();
    int from = (paramCount > 2) ? GetFuncParam(2).toInt() : 0;

    SetReturnVal(str.lastIndexOf(substring, from));
}

/**
 * @brief Заменяет все вхождения подстроки на другую подстроку.
 * @param str Исходная строка.
 * @param before Подстрока для замены.
 * @param after Новая подстрока.
 * @return Строка с выполненной заменой.
 * @see QString::replace()
 */
void strReplace()
{
    if (GetFuncParamCount() < 3)
        ThrowParamCountError(3);

    QString str = GetFuncParam(0).toString();
    QString before = GetFuncParam(1).toString();
    QString after = GetFuncParam(2).toString();

    SetReturnVal(str.replace(before, after));
}

/**
 * @brief Вставляет подстроку в указанную позицию.
 * @param str Исходная строка.
 * @param position Позиция для вставки.
 * @param substring Подстрока для вставки.
 * @return Строка с вставленной подстрокой.
 * @see QString::insert()
 */
void strInsert()
{
    if (GetFuncParamCount() < 3)
        ThrowParamCountError(3);

    QString str = GetFuncParam(0).toString();
    int position = GetFuncParam(1).toInt();
    QString substring = GetFuncParam(2).toString();

    SetReturnVal(str.insert(position, substring));
}

/**
 * @brief Удаляет часть строки.
 * @param str Исходная строка.
 * @param position Позиция, с которой начинать удаление.
 * @param n Количество символов для удаления.
 * @return Строка с удаленными символами.
 * @see QString::remove()
 */
void strRemove()
{
    if (GetFuncParamCount() < 3)
        ThrowParamCountError(3);

    QString str = GetFuncParam(0).toString();
    int position = GetFuncParam(1).toInt();
    int n = GetFuncParam(2).toInt();

    SetReturnVal(str.remove(position, n));
}

/**
 * @brief Добавляет строку в конец.
 * @param str Исходная строка.
 * @param substring Подстрока для добавления.
 * @return Строка с добавленной подстрокой.
 * @see QString::append()
 */
void strAppend()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString substring = GetFuncParam(1).toString();

    SetReturnVal(str.append(substring));
}

/**
 * @brief Добавляет строку в начало.
 * @param str Исходная строка.
 * @param substring Подстрока для добавления.
 * @return Строка с добавленной подстрокой.
 * @see QString::prepend()
 */
void strPrepend()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString substring = GetFuncParam(1).toString();

    SetReturnVal(str.prepend(substring));
}

/**
 * @brief Возвращает подстроку с указанной позиции и длины.
 * @param str Исходная строка.
 * @param position Позиция начала подстроки.
 * @param n Длина подстроки (по умолчанию до конца строки).
 * @return Подстрока.
 * @see QString::mid()
 */
void strMid()
{
    int paramCount = GetFuncParamCount();
    if (paramCount < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    int position = GetFuncParam(1).toInt();
    int n = (paramCount > 2) ? GetFuncParam(2).toInt() : -1;

    SetReturnVal(str.mid(position, n));
}

/**
 * @brief Возвращает указанное количество символов с начала строки.
 * @param str Исходная строка.
 * @param n Количество символов.
 * @return Подстрока.
 * @see QString::left()
 */
void strLeft()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    int n = GetFuncParam(1).toInt();

    SetReturnVal(str.left(n));
}

/**
 * @brief Возвращает указанное количество символов с конца строки.
 * @param str Исходная строка.
 * @param n Количество символов.
 * @return Подстрока.
 * @see QString::right()
 */
void strRight()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    int n = GetFuncParam(1).toInt();

    SetReturnVal(str.right(n));
}

/**
 * @brief Преобразует строку в целое число.
 * @param str Исходная строка.
 * @return Целое число или 0, если преобразование не удалось.
 * @see QString::toInt()
 */
void strToInt()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    bool ok;
    int result = str.toInt(&ok);

    SetReturnVal(ok ? result : 0);
}

/**
 * @brief Преобразует строку в число с плавающей точкой.
 * @param str Исходная строка.
 * @return Число с плавающей точкой или 0.0, если преобразование не удалось.
 * @see QString::toDouble()
 */
void strToDouble()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    bool ok;
    double result = str.toDouble(&ok);

    SetReturnVal(ok ? result : 0.0);
}

/**
 * @brief Возвращает длину строки.
 * @param str Исходная строка.
 * @return Длина строки.
 * @see QString::length()
 */
void strLength()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.length());
}

/**
 * @brief Проверяет, пуста ли строка.
 * @param str Исходная строка.
 * @return true, если строка пуста, иначе false.
 * @see QString::isEmpty()
 */
void strIsEmpty()
{
    if (GetFuncParamCount() < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    SetReturnVal(str.isEmpty());
}

/**
 * @brief Разделяет строку на части по разделителю.
 * @param str Исходная строка.
 * @param delimiter Разделитель (по умолчанию пробел).
 * @return Список строк.
 * @see QString::split()
 */
void strSplit()
{
    int paramCount = GetFuncParamCount();
    if (paramCount < 1)
        ThrowParamCountError(1);

    QString str = GetFuncParam(0).toString();
    QString delimiter = (paramCount > 1) ? GetFuncParam(1).toString() : " ";

    SetReturnVal(str.split(delimiter));
}

/**
 * @brief Подставляет значения в строку с плейсхолдерами.
 * @param str Исходная строка с плейсхолдерами.
 * @param arg Значение для подстановки.
 * @return Строка с подставленным значением.
 * @see QString::arg()
 */
void strArg()
{
    if (GetFuncParamCount() < 2)
        ThrowParamCountError(2);

    QString str = GetFuncParam(0).toString();
    QString arg = GetFuncParam(1).toString();

    SetReturnVal(str.arg(arg));
}

// -----------------------------------------------------------------------------

StrStaticModule::StrStaticModule() :
    RslStaticModule()
{
}

void StrStaticModule::Init()
{

}

void StrStaticModule::Proc()
{
    RegisterObjList::inst()->AddStdProc("strToUpper", strToUpper);
    RegisterObjList::inst()->AddStdProc("strToLower", strToLower);
    RegisterObjList::inst()->AddStdProc("strTrimmed", strTrimmed);
    RegisterObjList::inst()->AddStdProc("strSimplified", strSimplified);
    RegisterObjList::inst()->AddStdProc("strStartsWith", strStartsWith);
    RegisterObjList::inst()->AddStdProc("strEndsWith", strEndsWith);
    RegisterObjList::inst()->AddStdProc("strContains", strContains);
    RegisterObjList::inst()->AddStdProc("strIndexOf", strIndexOf);
    RegisterObjList::inst()->AddStdProc("strLastIndexOf", strLastIndexOf);
    RegisterObjList::inst()->AddStdProc("strReplace", strReplace);
    RegisterObjList::inst()->AddStdProc("strInsert", strInsert);
    RegisterObjList::inst()->AddStdProc("strRemove", strRemove);
    RegisterObjList::inst()->AddStdProc("strAppend", strAppend);
    RegisterObjList::inst()->AddStdProc("strPrepend", strPrepend);
    RegisterObjList::inst()->AddStdProc("strMid", strMid);
    RegisterObjList::inst()->AddStdProc("strLeft", strLeft);
    RegisterObjList::inst()->AddStdProc("strRight", strRight);
    RegisterObjList::inst()->AddStdProc("strToInt", strToInt);
    RegisterObjList::inst()->AddStdProc("strToDouble", strToDouble);
    RegisterObjList::inst()->AddStdProc("strLength", strLength);
    RegisterObjList::inst()->AddStdProc("strIsEmpty", strIsEmpty);
    RegisterObjList::inst()->AddStdProc("strSplit", strSplit);
    RegisterObjList::inst()->AddStdProc("strArg", strArg);
    RegisterObjList::inst()->AddStdProc("strFill", strFill);
}
