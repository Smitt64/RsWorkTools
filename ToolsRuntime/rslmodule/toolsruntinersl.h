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
};

#endif // TOOLSRUNTINERSL_H
