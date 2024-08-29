// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntime.h"
#include <Windows.h>
#include <QDir>
#include <QApplication>
#include <QSettings>
#include <QPluginLoader>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QVBoxLayout>
#include <QSyntaxHighlighter>
#include <QLoggingCategory>
#include <codeeditor/codehighlighter.h>
#include <codeeditor/codeeditor.h>
#include <functional>

Q_LOGGING_CATEGORY(logUnknown, "Unknown")
Q_LOGGING_CATEGORY(logHighlighter, "HighlighterStyle")
Q_LOGGING_CATEGORY(logRsl, "Rsl")
Q_LOGGING_CATEGORY(logSql, "Sql")
Q_LOGGING_CATEGORY(logSettings, "Settings")

Q_IMPORT_PLUGIN(RslToolsRuntimeModule)

typedef std::reference_wrapper<const QLoggingCategory> LoggingCategoryRef;
typedef std::pair<QString, LoggingCategoryRef> LoggingCategoryPair;
static std::map<QString, LoggingCategoryRef> InitCategoryList()
{
    std::map<QString, LoggingCategoryRef> _map;
    _map.insert(LoggingCategoryPair("HighlighterStyle", std::cref(logHighlighter())));
    _map.insert(LoggingCategoryPair("Rsl", std::cref(logRsl())));
    _map.insert(LoggingCategoryPair("Sql", std::cref(logSql())));
    return _map;
}

static bool m_fLogging = false;
static QScopedPointer<QFile> m_logFile;
static std::map<QString, LoggingCategoryRef> m_Category = InitCategoryList();

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }

    out << context.category << ": "
        << msg << Qt::endl;

    out.flush();
}

QString toolLogginFileName()
{
    if (!m_logFile.isNull() && m_logFile->isOpen())
        return m_logFile->fileName();

    return QString();
}

bool toolIsLoggingEnabled()
{
    return m_fLogging;
}

void toolDisableLogging()
{
    if (!m_logFile.isNull() && m_logFile->isOpen())
    {
        m_logFile->close();
        m_logFile.reset(Q_NULLPTR);
        qInstallMessageHandler(Q_NULLPTR);
        m_fLogging = false;
    }
}

bool toolInitLogging(const QString &prefix, const QString &rules)
{
    QDir logDir(qApp->applicationDirPath());

    if (!logDir.cd("logs"))
    {
        logDir.mkdir("logs");
        logDir.cd("logs");
    }

    QString baseName = prefix + QString("_") + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz");
    QString logFileName = QString("%1.txt")
                              .arg(logDir.absoluteFilePath(baseName))
                              .arg(prefix);

    m_logFile.reset(new QFile(logFileName));

    if (m_logFile->open(QFile::Append | QFile::Text))
    {
        toolSetLoggingRules(rules);
        qInstallMessageHandler(messageHandler);
        m_fLogging = true;
    }
    else
    {
        m_logFile.reset(Q_NULLPTR);
        m_fLogging = false;
    }

    return m_fLogging;
}

void toolLoggingCategoryListAdd(QLoggingCategory &category)
{
    m_Category.insert(LoggingCategoryPair(QLatin1String(category.categoryName()),
                                 std::cref(category)));
}

QStringList toolLoggingCategoryList()
{
    QStringList list;
    for (auto it = m_Category.begin(); it != m_Category.end(); ++it)
        list.append(it->first);

    return list;
}

const QLoggingCategory &toolLoggingCategory(const QString &name)
{
    for (auto it = m_Category.begin(); it != m_Category.end(); ++it)
    {
        if (it->first == name)
            return it->second.get();
    }

    return logUnknown();
}

void toolSetLoggingRules(const QString &rules)
{
    if (!rules.isEmpty())
    {
        QString r = rules;
        QLoggingCategory::setFilterRules(r.replace(";", "\n"));
    }
}

// --------------------------------------------------------

QString toolFullFileNameFromDir(const QString &file)
{
    QDir dir = QDir::current();
    QString fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    dir = QDir(QApplication::applicationDirPath());
    fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    return QString();
}

QString toolReadTextFileContent(const QString &filename, const QString &encode)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (encode.isEmpty())
            content = f.readAll();
        else
        {
            QTextStream stream(&f);
            stream.setCodec(encode.toLocal8Bit().data());

            content = stream.readAll();
        }

        f.close();
    }
    return content;
}

QByteArray toolReadFileContent(const QString &filename)
{
    QByteArray content;
    QFile f(filename);

    if (f.open(QIODevice::ReadOnly))
    {
        content = f.readAll();
        f.close();
    }

    return content;
}

bool toolSaveResourceToFile(const QString &resname, const QString &filename) 
{ 
    QFile res(resname), out(filename); 
    if (res.open(QIODevice::ReadOnly)) 
    { 
        if (out.open(QIODevice::WriteOnly)) 
        { 
            out.write(res.readAll()); 
            out.close(); 
        } 
        else 
        { 
            res.close(); 
            return false; 
        } 
        res.close(); 
 
        return true; 
    } 
    return false; 
} 

bool toolGetPostgreSQLInstallLocation(QDir &dir)
{
    QSettings settings64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", QSettings::NativeFormat);
    QStringList childKeys = settings64.childGroups();

    for (const QString &key : childKeys)
    {
        if (key.contains("PostgreSQL"))
        {
            settings64.beginGroup(key);
            QString InstallLocation = settings64.value("InstallLocation").toString();

            if (!InstallLocation.isEmpty())
            {
                QDir tmp(InstallLocation);

                if (tmp.cd("bin"))
                {
                    if (QFile::exists(tmp.absoluteFilePath("pg_dump.exe")) &&
                        QFile::exists(tmp.absoluteFilePath("psql.exe")))
                    {
                        dir = tmp;
                        settings64.endGroup();
                        return true;
                    }
                }
            }
            settings64.endGroup();
        }
    }
    return false;
}

QString toolGetRuntimeVersion()
{
    QString versionNumberString;
    char moduleName[MAX_PATH + 1];

    HMODULE hm = NULL;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                       GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPWSTR) &toolGetRuntimeVersion, &hm);

    GetModuleFileNameA(hm, moduleName, MAX_PATH);

    DWORD dummyZero;
    DWORD versionSize = GetFileVersionInfoSizeA(moduleName, &dummyZero);

    if(!versionSize)
        return QString();

    void* pVersion = malloc(versionSize);
    if(!pVersion)
        return QString();

    if(!GetFileVersionInfoA(moduleName, NULL, versionSize, pVersion))
    {
        free(pVersion);
        return QString();
    }

    UINT length;
    VS_FIXEDFILEINFO* pFixInfo;
    if (VerQueryValueA(pVersion, "\\", (LPVOID*)&pFixInfo, &length))
    {
        versionNumberString = QString("%1.%2.%3.%4")
                .arg((pFixInfo->dwFileVersionMS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionMS >>  0) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >>  0) & 0xffff);
    }
    free(pVersion);

    return versionNumberString;
}

int toolExecuteQuery(QSqlQuery *query, QString *err)
{
    int stat = 0;

    QMap<QString, QVariant> values = query->boundValues();
    QMapIterator<QString, QVariant> i(values);
    while(i.hasNext())
    {
        i.next();
        qCInfo(logSql()) << i.key() << ": " << i.value();
    }

    bool result = query->exec();
    if (!result)
    {
        stat = 1;
        qCCritical(logSql()) << query->lastError().text();

        if (err != Q_NULLPTR)
            *err = query->lastError().text();
    }
    qCInfo(logSql()) << query->executedQuery();
    qCInfo(logSql()) << "Result:" << result;

    return stat;
}

int toolShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code)
{
    int result = 0;
    QDialog dlg(parent);
    CodeEditor *editor = new CodeEditor(&dlg);
    QVBoxLayout *main = new QVBoxLayout(&dlg);
    editor->setReadOnly(true);
    main->addWidget(editor);
    dlg.setMinimumSize(640, 480);
    dlg.setWindowTitle(title);
    dlg.setLayout(main);

    editor->setPlainText(code);

    ToolApplyHighlighter(editor, type);

    result = dlg.exec();

    return result;
}

int toolHighlighterByName(const QString &name)
{
    if (!name.compare("HighlighterCpp", Qt::CaseInsensitive) ||
        !name.compare("Cpp", Qt::CaseInsensitive))
    {
        return HighlighterCpp;
    }
    else if (!name.compare("HighlighterRsl", Qt::CaseInsensitive) ||
               !name.compare("Rsl", Qt::CaseInsensitive))
    {
        return HighlighterRsl;
    }
    else if (!name.compare("HighlighterSql", Qt::CaseInsensitive) ||
             !name.compare("Sql", Qt::CaseInsensitive))
    {
        return HighlighterSql;
    }
    else if (!name.compare("HighlighterXml", Qt::CaseInsensitive) ||
             !name.compare("xml", Qt::CaseInsensitive))
    {
        return HighlighterXml;
    }

    return HighlighterPlain;
}
