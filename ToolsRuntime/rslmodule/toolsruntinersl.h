// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTINERSL_H
#define TOOLSRUNTINERSL_H

#include <QObject>
#include <QDir>

/**
 * @brief Класс для работы с инструментальной библиотекой
 */
class ToolsRuntime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rslLibVersion READ GetRuntimeVersion);
public:
    /**
     * @brief Конструктор ToolsRuntime
     * @param родитель
     */
    Q_INVOKABLE ToolsRuntime(QObject *parent = nullptr);

    /**
     * @brief Возвращает место установки PostgreSQL
     * @param директория
     * @return
     */
    Q_INVOKABLE bool GetPostgreSQLInstallLocation(QString &dir);

    /**
     * @brief Возвращает содержимое текстового файла
     * @param имя_файла
     * @return
     */
    Q_INVOKABLE QString ReadTextFileContent(const QString &filename, const QString &encode = QString());

    /**
     * @brief Возвращает версию инструментальной библиотеки
     * @return
     */
    Q_INVOKABLE QString GetRuntimeVersion() const;

    /**
     * @brief Возвращает полный путь к файлу
     * @param имя_файла
     * @return
     */
    Q_INVOKABLE QString FullFileNameFromDir(const QString &file);

    /**
     * @brief Возвращает идентификатор подсветки по имени
     * @param имя
     * @return
     */
    Q_INVOKABLE int HighlighterByName(const QString &name);

    Q_INVOKABLE QString toString(const QTime &time, const QString &format);
    Q_INVOKABLE QString toString(const QDate &date, const QString &format);

    Q_INVOKABLE QStringList split(const QString &str, const QString &sep);
    Q_INVOKABLE QString join(const QStringList &lst, const QString &separator);

    Q_INVOKABLE QString number(int n, int base = 10);
    Q_INVOKABLE QString number(qint64 n, int base = 10);
    Q_INVOKABLE QString number(double n, char format = 'g', int precision = 6);

    Q_INVOKABLE QString replace(QString str, const int &pos, const int &len, const QString &replace);
    Q_INVOKABLE QString replace(QString str, const QString &before, const QString &after, const bool &CaseSensitive = true);
    Q_INVOKABLE QString simplified(QString str);

    Q_INVOKABLE QStringList getJavaHomes(const quint32 &homeflags);

    Q_INVOKABLE int levenshteinDistance(const QString &s1, const QString &s2);
    Q_INVOKABLE double levenshteinSimilarity(const QString &s1, const QString &s2);

    Q_INVOKABLE bool CopyDirectory(const QString &sourceDir, const QString &destinationDir);

    Q_INVOKABLE QString applicationDirPath();
    Q_INVOKABLE QString applicationFilePath();
    Q_INVOKABLE QString applicationName();
    Q_INVOKABLE qint64 applicationPid();
    Q_INVOKABLE QString applicationVersion();
    Q_INVOKABLE QStringList arguments();
    Q_INVOKABLE QString platformName();

    Q_INVOKABLE int makeArchive(const QString &archiveName, const QString &sourceDir);
    Q_INVOKABLE int makeArchive(const QString &archiveName, const QStringList &files);
    Q_INVOKABLE int extractArchive(const QString &archiveName, const QString &extractDir);
    Q_INVOKABLE int extractDirFromArchive(const QString &archiveName, const QString &targetDir, const QString &dirInArchive);
};

#endif // TOOLSRUNTINERSL_H
