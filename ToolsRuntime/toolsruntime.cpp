// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntime.h"
#include "rslmodule/sql/sqldatabase.h"
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
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QDirIterator>
#include <QDomDocument>

Q_LOGGING_CATEGORY(logUnknown, "Unknown")
Q_LOGGING_CATEGORY(logHighlighter, "HighlighterStyle")
Q_LOGGING_CATEGORY(logRsl, "Rsl")
Q_LOGGING_CATEGORY(logSql, "Sql")
Q_LOGGING_CATEGORY(logSettings, "Settings")
Q_LOGGING_CATEGORY(logProcess, "Process")
Q_LOGGING_CATEGORY(logUpdate, "Update")
Q_LOGGING_CATEGORY(logNetApi, "logNetApi")

Q_IMPORT_PLUGIN(RslToolsRuntimeModule)
Q_GLOBAL_STATIC(QString, ApiBaseUrl)

typedef std::reference_wrapper<const QLoggingCategory> LoggingCategoryRef;
typedef std::pair<QString, LoggingCategoryRef> LoggingCategoryPair;
static std::map<QString, LoggingCategoryRef> InitCategoryList()
{
    std::map<QString, LoggingCategoryRef> _map;
    _map.insert(LoggingCategoryPair("HighlighterStyle", std::cref(logHighlighter())));
    _map.insert(LoggingCategoryPair("Rsl", std::cref(logRsl())));
    _map.insert(LoggingCategoryPair("Sql", std::cref(logSql())));
    _map.insert(LoggingCategoryPair("Process", std::cref(logProcess())));
    _map.insert(LoggingCategoryPair("Update", std::cref(logUpdate())));
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

void toolLoggingCategoryListAdd(const QLoggingCategory &category)
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

void toolMakeSqlDatabaseObj(QSqlDatabase &db, QObject **obj)
{
    *obj = new SqlDatabase(db);
}

QString toolGetProcessErrorText(const QProcess::ProcessError &error)
{
    QString errText;
    switch(error)
    {
    case QProcess::FailedToStart:
        errText = "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program";
        break;
    case QProcess::Crashed:
        errText = "The process crashed some time after starting successfully";
        break;
    case QProcess::Timedout:
        errText = "The process timedout";
        break;
    case QProcess::WriteError:
        errText = "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel";
        break;
    case QProcess::ReadError:
        errText = "An error occurred when attempting to read from the process. For example, the process may not be running";
        break;
    case QProcess::UnknownError:
        errText = "An unknown error occurred";
        break;
    }

    return errText;
}

QString toolProcessStateText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::Starting:
        result = "The process is starting, but the program has not yet been invoked.";
        break;
    case QProcess::Running:
        result = "The process is running and is ready for reading and writing.";
        break;
    default:
        result = "The process is not running.";
    }

    return result;
}

QString toolProcessExitStatusText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::CrashExit:
        result = "The process crashed.";
        break;
    default:
        result = "The process exited normally.";
    }

    return result;
}

int toolStartProcess(QProcess *exe, const QString &program,
                         const QStringList& arguments,
                         bool waitForFinished,
                         bool waitForStarted,
                         int timeout,
                         bool waitForReadyRead)
{
    int stat = 0;
    qCInfo(logProcess()) << "Process: " << exe;
    qCInfo(logProcess()) << "Executable path:" << program;
    qCInfo(logProcess()) << "Working directory:" << exe->workingDirectory();
    qCInfo(logProcess()) << "Arguments" << arguments;

    QObject::connect(exe, &QProcess::stateChanged, [&exe](QProcess::ProcessState newState)
                     {
                         qCInfo(logProcess()) << QString("Process state changed to: %1 (%2)")
                         .arg(newState).arg(toolProcessStateText(newState));
                     });
    QObject::connect(exe, &QProcess::errorOccurred, [&exe,&stat](QProcess::ProcessError error)
                     {
                         qCInfo(logProcess()) << QString("Process error occurred: %1 (%2)")
                         .arg(error).arg(toolGetProcessErrorText(error));
                         stat = -1;
                     });
    exe->start(program, arguments);

    if (waitForStarted)
        exe->waitForStarted();

    if (waitForReadyRead)
        exe->waitForReadyRead();

    if (waitForFinished)
    {
        exe->waitForFinished(timeout);

        if (!stat)
        {
            stat = exe->exitCode();
            qCInfo(logProcess()) << "Process exit code: " << stat;
        }

        int status= exe->exitStatus();
        qCInfo(logProcess()) << QString("Process exit status: %1 (%2)").arg(status).arg(toolProcessExitStatusText(status));
    }

    return stat;
}

// ---------------------------------------------------------------------
/*BOOL GetCurrentUserAndDomain(PTSTR szUser, PDWORD pcchUser, PTSTR szDomain, PDWORD pcchDomain)
{
    BOOL         fSuccess = FALSE;
    HANDLE       hToken   = NULL;
    PTOKEN_USER  ptiUser  = NULL;
    DWORD        cbti     = 0;
    SID_NAME_USE snu;

    __try
    {
        if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE,&hToken))
        {
            if (GetLastError() != ERROR_NO_TOKEN)
                __leave;

            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
                __leave;
        }
        if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti))
            __leave;
        else
        {
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                __leave;
        }

        ptiUser = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbti);
        if (!ptiUser)
            __leave;

        if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
            __leave;

        if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, pcchUser, szDomain, pcchDomain, &snu))
            __leave;

        fSuccess = TRUE;

    } __finally
    {
        if (hToken)
            CloseHandle(hToken);

        if (ptiUser)
            HeapFree(GetProcessHeap(), 0, ptiUser);
    }

    return fSuccess;
}*/

/*UserDomainTuple toolGetCurrentUserAndDomain()
{
    UserDomainTuple result;

    TCHAR user[1024], domain[1024];
    DWORD chUser = sizeof(user), chDomain = sizeof(domain);

    if (GetCurrentUserAndDomain(user, &chUser, domain, &chDomain))
        result = std::make_tuple(QString::fromWCharArray(user), QString::fromWCharArray(domain));

    return result;
}*/

QString toolGetApiUrl(const QString &method)
{
    /*if (!ApiBaseUrl->isEmpty())
        return QString("%1%2").arg(*ApiBaseUrl, method);

    QString hosts = toolFullFileNameFromDir("host.api");
    if (!hosts.isEmpty() && QFile::exists(hosts))
    {
        QFile f(hosts);
        if (f.open(QIODevice::ReadOnly))
        {
            QString host = f.readLine();

            if (!host.isEmpty())
                *ApiBaseUrl = host;
            else
                *ApiBaseUrl = API_URL_BASE;

            f.close();
        }
    }
    else
        *ApiBaseUrl = API_URL_BASE;

    return QString("%1%2").arg(*ApiBaseUrl, method);*/
    return QString();
}

QNetworkReply *toolGetApiReply(QNetworkRequest *request, QNetworkAccessManager *ApiManager, bool wait)
{
    QNetworkReply *Reply = ApiManager->get(*request);

    if (wait)
    {
        QEventLoop loop;
        QObject::connect(Reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    return Reply;
}

QNetworkReply *toolPostApiReply(QNetworkRequest *request, QNetworkAccessManager *ApiManager, const QByteArray &data, bool wait)
{
    QNetworkReply *Reply = ApiManager->post(*request, data);

    if (wait)
    {
        QEventLoop loop;
        QObject::connect(Reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    return Reply;
}

QStringList toolGetJavaHomes(const quint32 &homeflags)
{
    QSet<QString> homes;

    auto ReadPath = [&homes](const QString &path)
    {
        QSettings settings64(path, QSettings::NativeFormat);
        QStringList childKeys = settings64.childGroups();

        for (const QString &sub : childKeys)
        {
            QSettings subreg(QString("%1\\%2").arg(path).arg(sub), QSettings::NativeFormat);
            homes.insert(QDir::toNativeSeparators(subreg.value("JavaHome").toString()));
        }
    };

    auto ReadProgramFiles = [&homes](const QString &path, const quint32 &homeflags)
    {
        QDirIterator iter(QString("%1\\Java").arg(path), { "javac.exe" }, QDir::Files, QDirIterator::Subdirectories);
        if (iter.hasNext())
        {
            QString d = iter.next();
            QDir dir(d);
            dir.cdUp();
            dir.cdUp();

            QString absolutePath = dir.absolutePath();

            if ((homeflags & JavaRuntimeHomes32) && (homeflags & JavaRuntimeHomes64))
            {
                if (absolutePath.contains("jre1", Qt::CaseInsensitive))
                    homes.insert(QDir::toNativeSeparators(absolutePath));
            }

            if ((homeflags & JavaDevelopmentHomes32) && (homeflags & JavaDevelopmentHomes64))
            {
                if (absolutePath.contains("jdk", Qt::CaseInsensitive))
                    homes.insert(QDir::toNativeSeparators(absolutePath));
            }
        }
    };

    ReadProgramFiles(qgetenv("ProgramFiles(x86)"), homeflags);
    ReadProgramFiles(qgetenv("ProgramW6432"), homeflags);

    if (homeflags & JavaRuntimeHomes32)
        ReadPath("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\JavaSoft\\Java Runtime Environment");

    if (homeflags & JavaDevelopmentHomes32)
        ReadPath("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\JavaSoft\\Java Development Kit");

    return homes.values();
}

SvnInfoMap toolSvnGetRepoInfo(const QString &path)
{
    SvnInfoMap info;

    QProcess proc;
    proc.setWorkingDirectory(path);

    toolStartProcess(&proc, "svn.exe", {"info", "--xml"}, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QDomDocument doc;
    doc.setContent(data);

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChildElement("entry").firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if (e.tagName() != "wc-info")
            info.insert(e.tagName(), e.text());

        n = n.nextSibling();
    }

    return info;
}

SvnInfoMap toolGitGetRepoInfo(const QString &path)
{
    SvnInfoMap info;

    QProcess proc;
    proc.setWorkingDirectory(path);

    // Получаем основную информацию о репозитории
    toolStartProcess(&proc, "git.exe", {"remote", "get-url", "origin"}, true, true, 30000, true);
    QString remoteUrl = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();

    // Получаем текущую ветку
    toolStartProcess(&proc, "git.exe", {"branch", "--show-current"}, true, true, 30000, true);
    QString currentBranch = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();

    // Получаем последний коммит
    toolStartProcess(&proc, "git.exe", {"log", "-1", "--pretty=format:%H|%an|%ae|%ad|%s", "--date=iso"}, true, true, 30000, true);
    QString lastCommit = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();

    // Получаем корневую директорию репозитория
    toolStartProcess(&proc, "git.exe", {"rev-parse", "--show-toplevel"}, true, true, 30000, true);
    QString repoRoot = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();

    // Получаем относительный путь внутри репозитория
    toolStartProcess(&proc, "git.exe", {"rev-parse", "--show-prefix"}, true, true, 30000, true);
    QString relativePath = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();

    // Заполняем информацию в формате, аналогичном SVN
    info.insert("url", remoteUrl);
    info.insert("relative-url", relativePath.isEmpty() ? "/" : "/" + relativePath);
    info.insert("repository-root", repoRoot);
    info.insert("branch", currentBranch);

    if (!lastCommit.isEmpty())
    {
        QStringList commitParts = lastCommit.split('|');
        if (commitParts.size() >= 5)
        {
            info.insert("commit", commitParts[0]); // Хеш коммита
            info.insert("author", commitParts[1]); // Автор
            info.insert("last-changed-author", commitParts[1]);
            info.insert("last-changed-date", commitParts[3]); // Дата
        }
    }

    // Получаем информацию о последнем коммите для текущей директории
    toolStartProcess(&proc, "git.exe", {"log", "-1", "--pretty=format:%H", "--", "."}, true, true, 30000, true);
    QString lastCommitForPath = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
    info.insert("revision", lastCommitForPath); // Используем хеш как ревизию

    return info;
}

VcsType toolDetectVcsType(const QString &path)
{
    QDir dir(path);

    if (dir.exists(".git"))
        return VcsType::Git;

    // Проверяем родительские каталоги для Git
    QString currentPath = path;
    while (!currentPath.isEmpty() && QDir(currentPath).exists())
    {
        QDir currentDir(currentPath);
        if (currentDir.exists(".git"))
            return VcsType::Git;

        if (currentPath == path && currentDir.exists(".svn"))
            return VcsType::Svn;

        QString parentPath = QDir(currentPath).absolutePath();
        if (parentPath == currentPath) break;
        currentPath = QDir(currentPath).absoluteFilePath("..");
        if (currentPath == parentPath) break;
    }

    if (dir.exists(".svn"))
        return VcsType::Svn;

    return VcsType::None;
}

SvnInfoMap toolVcsGetRepoInfo(const QString &path)
{
    // Сначала проверяем, это Git репозиторий
    QDir dir(path);
    if (dir.exists(".git"))
    {
        return toolGitGetRepoInfo(path);
    }

    // Проверяем родительские каталоги для Git
    QString currentPath = path;
    while (!currentPath.isEmpty() && QDir(currentPath).exists())
    {
        QDir currentDir(currentPath);
        if (currentDir.exists(".git"))
        {
            return toolGitGetRepoInfo(currentPath);
        }

        // Проверяем SVN в текущем каталоге (только для исходного пути)
        if (currentPath == path && currentDir.exists(".svn"))
        {
            return toolSvnGetRepoInfo(path);
        }

        QString parentPath = QDir(currentPath).absolutePath();
        if (parentPath == currentPath) break;
        currentPath = QDir(currentPath).absoluteFilePath("..");
        if (currentPath == parentPath) break;
    }

    // Если это SVN репозиторий
    if (dir.exists(".svn"))
    {
        return toolSvnGetRepoInfo(path);
    }

    // Если репозиторий не найден, возвращаем пустую карту
    return SvnInfoMap();
}

QString toolReplaceUnicodeSymToOem(const QString &text)
{
    if (text.isEmpty())
        return text;

    QString result = text;

    // Заменяем проблемные символы для CP866
    QHash<QChar, QChar> symbolReplacements = {
        {QChar(0x2013), '-'},    // Длинное тире → обычный дефис
        {QChar(0x2014), '-'},    // Длинное тире → обычный дефис
        {QChar(0x2018), '\''},   // Левая одинарная кавычка → обычная кавычка
        {QChar(0x2019), '\''},   // Правая одинарная кавычка → обычная кавычка
        {QChar(0x201C), '\"'},   // Левая двойная кавычка → обычная кавычка
        {QChar(0x201D), '\"'},   // Правая двойная кавычка → обычная кавычка
        {QChar(0x00AB), '\"'},   // Левая угловая кавычка → обычная кавычка
        {QChar(0x00BB), '\"'},   // Правая угловая кавычка → обычная кавычка
        {QChar(0x2039), '\''},   // Левая одинарная угловая кавычка → обычная кавычка
        {QChar(0x203A), '\''},   // Правая одинарная угловая кавычка → обычная кавычка
        {QChar(0x2026), '.'},    // Многоточие → три точки (обработаем отдельно)
        {QChar(0x201E), '\"'},   // Нижняя двойная кавычка → обычная кавычка
        {QChar(0x2E3A), '-'},    // Двойное длинное тире → обычный дефис
        {QChar(0x2E3B), '-'},    // Тройное длинное тире → обычный дефис
    };

    // Замена символов из хэша
    for (auto it = symbolReplacements.begin(); it != symbolReplacements.end(); ++it)
    {
        result.replace(it.key(), it.value());
    }

    // Специальная обработка для многоточия
    result.replace(QString(QChar(0x2026)), "...");

    // Дополнительно: заменяем "умные" кавычки, которые могут быть в тексте
    result.replace("«", "\"");
    result.replace("»", "\"");
    result.replace("„", "\"");
    result.replace("“", "\"");
    result.replace("‟", "\"");
    result.replace("”", "\"");
    result.replace("‘", "'");
    result.replace("’", "'");
    result.replace("‛", "'");
    result.replace("‚", "'");

    return result;
}
