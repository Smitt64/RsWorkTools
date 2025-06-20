#include "toolsqlconverter.h"
#include <QGlobalStatic>
#include <QLibrary>

typedef void (*ConvertFuncPtr)(const wchar_t* Sql, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);

Q_GLOBAL_STATIC_WITH_ARGS(QLibrary, globalPgConvLib, ("PgConvWrapper.dll"))

QLibrary* globalPgConvWrapper()
{
    QLibrary* lib = globalPgConvLib();
    lib->setLoadHints(QLibrary::ResolveAllSymbolsHint);

    return lib;
}

// ---------------------------------------------------------------------------

SqlConversionResult convertSql(const QString &sql)
{
    SqlConversionResult result;

    QLibrary* lib = globalPgConvWrapper();
    if (!lib)
    {
        result.error = lib->errorString();
        return result;
    }

    auto func = reinterpret_cast<ConvertFuncPtr>(lib->resolve("convertSqlOraToPg"));

    if (!func)
    {
        result.error = "Failed to resolve convertSqlOraToPg";
        return result;
    }

    wchar_t *PgSql = nullptr;
    wchar_t *Error = nullptr;
    wchar_t *Tail = nullptr;

    func(sql.toStdWString().c_str(), &PgSql, &Error, &Tail);

    result.result = QString::fromWCharArray(PgSql);
    result.error = QString::fromWCharArray(Error);
    result.tail = QString::fromWCharArray(Tail);

    free(PgSql);
    free(Error);
    free(Tail);

    return result;
}

// ---------------------------------------------------------------------------

SqlConverter::SqlConverter(QObject *parent)
    : QObject{parent}
{

}

SqlConverter::~SqlConverter()
{

}
