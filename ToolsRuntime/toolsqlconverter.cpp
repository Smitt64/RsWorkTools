#include "toolsqlconverter.h"
#include <QGlobalStatic>
#include <QLibrary>
#include <QRegularExpression>
#include <QTextStream>

typedef void (*ConvertFuncPtr)(const wchar_t* Sql, const wchar_t* User, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);

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

SqlConversionResult convertSql(const QString &sql, const QString &User)
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

    func(sql.toStdWString().c_str(), User.toStdWString().c_str(), &PgSql, &Error, &Tail);

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

QString SqlConverter::convert(const QString &sql, const QString &User)
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

    d->func(sql.toStdWString().c_str(), User.toStdWString().c_str(), &PgSql, &Error, &Tail);

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

// ---------------------------------------------------------------------------

ParsedSqlError parseAntlrError(const QString &error)
{
    ParsedSqlError result;
    result.isValid = false;

    if (error.isEmpty())
        return result;

    // Парсинг ошибки ANTLR
    // Пример: "Error. Expected <Tag: alternative Value: ...> at: <select 1 from [-- expected_pos -->1;>"

    // Извлекаем "Expected ..."
    QRegularExpression expectedRegex("Expected[\\s\\S]*?(?=at:)");
    QRegularExpressionMatch expectedMatch = expectedRegex.match(error);
    if (expectedMatch.hasMatch()) {
        QString expectedText = expectedMatch.captured(0);
        // Упрощаем expected текст
        expectedText.remove(QRegularExpression("<[^>]+>"));
        expectedText.replace("alternative", "");
        expectedText.replace("sequence", "");
        expectedText.replace("strlit", "");
        expectedText.replace(QRegularExpression("\\s+"), " ");
        result.expected = expectedText.trimmed();
    } else {
        result.expected = "неизвестная конструкция";
    }

    // Извлекаем "at: <...>"
    QRegularExpression atRegex("at:\\s*<([^>]+)>");
    QRegularExpressionMatch atMatch = atRegex.match(error);
    if (atMatch.hasMatch()) {
        QString atText = atMatch.captured(1);

        // Извлекаем найденный текст
        QRegularExpression foundRegex("select\\s+.*?(?=;|$)");
        QRegularExpressionMatch foundMatch = foundRegex.match(atText);
        if (foundMatch.hasMatch()) {
            result.found = foundMatch.captured(0).left(100);
            if (result.found.length() >= 100)
                result.found += "...";
        } else {
            result.found = atText.left(100);
        }

        // Извлекаем позицию ошибки
        QRegularExpression posRegex("expected_pos\\s*-->\\s*(\\d+)");
        QRegularExpressionMatch posMatch = posRegex.match(atText);
        if (posMatch.hasMatch()) {
            int pos = posMatch.captured(1).toInt();
            result.position = QString("позиция %1").arg(pos);
        } else {
            result.position = "в начале выражения";
        }

        result.context = atText.left(200);
        result.isValid = true;
    }

    return result;
}

QString formatSqlErrorMessage(const QString &error)
{
    if (error.isEmpty())
        return QString();

    ParsedSqlError parsed = parseAntlrError(error);

    if (!parsed.isValid) {
        // Если не удалось распарсить, возвращаем исходную ошибку
        return error;
    }

    QString formatted;
    QTextStream stream(&formatted);

    stream << QString::fromLocal8Bit("Ожидалась конструкция:\n");
    stream << "  " << parsed.expected << "\n\n";

    stream << QString::fromLocal8Bit("Найдено:\n");
    stream << "  " << parsed.found << "\n\n";

    stream << QString::fromLocal8Bit("Позиция ошибки:\n");
    stream << "  " << parsed.position << "\n\n";

    stream << QString::fromLocal8Bit("Контекст:\n");
    stream << "  " << parsed.context << "\n\n";

    return formatted;
}