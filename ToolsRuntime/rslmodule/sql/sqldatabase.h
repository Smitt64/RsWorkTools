#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QObject>

class QSqlDatabase;
/**
 * @brief Класс для работы с базой данных SQL
 */
class SqlDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError CONSTANT)
    Q_PROPERTY(QString connectionName READ connectionName CONSTANT)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(bool isOpen READ isOpen CONSTANT)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString connectOptions READ connectOptions WRITE setConnectOptions)
public:
    /**
     * @brief Конструктор SqlDatabase
     * @param драйвер
     */
    Q_INVOKABLE SqlDatabase(const QString& driver);
    SqlDatabase(QSqlDatabase& _db);

    /**
     * @brief Открывает соединение с базой данных
     * @return
     */
    Q_INVOKABLE bool open();

    /**
     * @brief Закрывает соединение с базой данных
     */
    Q_INVOKABLE void close();

    /**
     * @brief Возвращает имя соединения
     * @return
     */
    QString connectionName() const;

    /**
     * @brief Возвращает базу данных
     * @return
     */
    QSqlDatabase &database();

    /**
     * @brief Проверяет, открыто ли соединение с базой данных
     * @return
     */
    bool isOpen() const;

    /**
     * @brief Возвращает последнее сообщение об ошибке
     * @return
     */
    QString lastError() const;

    /**
     * @brief Устанавливает имя хоста
     * @param имя_хоста
     */
    void setHostName(const QString& hostName);

    /**
     * @brief Возвращает имя хоста
     * @return
     */
    QString hostName() const;

    /**
     * @brief Возвращает имя базы данных
     * @return
     */
    QString databaseName() const;

    /**
     * @brief Устанавливает имя базы данных
     * @param имя
     */
    void setDatabaseName(const QString& name);

    /**
     * @brief Устанавливает имя пользователя
     * @param имя_пользователя
     */
    void setUserName(const QString& userName);

    /**
     * @brief Возвращает имя пользователя
     * @return
     */
    QString userName() const;

    /**
     * @brief Устанавливает пароль
     * @param пароль
     */
    void setPassword(const QString& password);

    /**
     * @brief Возвращает пароль
     * @return
     */
    QString password() const;

    /**
     * @brief Устанавливает порт
     * @param порт
     */
    void setPort(int port);

    /**
     * @brief Возвращает порт
     * @return
     */
    int port() const;

    /**
     * @brief Устанавливает опции подключения
     * @param опции
     */
    void setConnectOptions(const QString& connectOptions);

    /**
     * @brief Возвращает опции подключения
     * @return
     */
    QString connectOptions() const;

private:
    QScopedPointer<QSqlDatabase> db;
};
;

#endif // SQLDATABASE_H
