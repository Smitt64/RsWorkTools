#include "toolsqlconverter.h"
#include <QGlobalStatic>
#include <QLibrary>

typedef void (*ConvertFuncPtr)(const wchar_t* Sql, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);

Q_GLOBAL_STATIC_WITH_ARGS(QLibrary, globalPgConvLib, ("PgConvWrapper.dll"))

QLibrary* globalPgConvWrapper()
{
    QLibrary* lib = globalPgConvLib();
    lib->setLoadHints(QLibrary::ResolveAllSymbolsHint);
    lib->load();

    return lib;
}

bool isSqlConverterAvailable()
{
    QLibrary* lib = globalPgConvWrapper();
    return lib->isLoaded();
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

class SqlConverterPrivate
{
public:
    typedef void (*ConvertFuncPtr)(const wchar_t* Sql, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);

    ConvertFuncPtr func = nullptr;
    QString m_lastError;
    QString m_tail;

    SqlConverterPrivate()
    {
        QLibrary* lib = globalPgConvWrapper();

        if (!lib->isLoaded())
            m_lastError = lib->errorString();
        else
        {
            func = reinterpret_cast<ConvertFuncPtr>(lib->resolve("convertSqlOraToPg"));
            if (!func)
                m_lastError = "Failed to resolve convertSqlOraToPg";
        }
    }

    ~SqlConverterPrivate()
    {
    }
};

SqlConverter::SqlConverter(QObject *parent)
    : QObject{parent},
      d_ptr(new SqlConverterPrivate)
{

}

SqlConverter::~SqlConverter()
{

}

QString SqlConverter::lastError() const
{
    Q_D(const SqlConverter);
    return d->m_lastError;
}

QString SqlConverter::tail() const
{
    Q_D(const SqlConverter);
    return d->m_tail;
}

QString SqlConverter::convert(const QString &sql)
{
    Q_D(SqlConverter);

    if (!d->func)
    {
        d->m_lastError = "Function not resolved";
        emit lastErrorChanged(d->m_lastError);
        return {};
    }

    wchar_t *PgSql = nullptr;
    wchar_t *Error = nullptr;
    wchar_t *Tail = nullptr;

    d->func(sql.toStdWString().c_str(), &PgSql, &Error, &Tail);

    QString result = QString::fromWCharArray(PgSql);
    d->m_lastError = QString::fromWCharArray(Error);
    d->m_tail = QString::fromWCharArray(Tail);

    emit lastErrorChanged(d->m_lastError);
    emit tailChanged(d->m_tail);

    free(PgSql);
    free(Error);
    free(Tail);

    return result;
}
