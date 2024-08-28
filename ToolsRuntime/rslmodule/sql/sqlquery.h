#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QObject>
#include <QVariant>

class QSqlQuery;
class SqlDatabase;
/**
 * @brief Класс для работы с запросами к базе данных SQL
 */
class SqlQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString executedQuery READ executedQuery CONSTANT)
    Q_PROPERTY(QString lastQuery READ lastQuery CONSTANT)
    Q_PROPERTY(QString lastError READ lastError CONSTANT)
    Q_PROPERTY(int numRowsAffected READ numRowsAffected CONSTANT)
    Q_PROPERTY(bool isActive READ isActive CONSTANT)
    Q_PROPERTY(bool isSelect READ isSelect CONSTANT)
    Q_PROPERTY(bool isValid READ isValid CONSTANT)
    Q_PROPERTY(int pos READ at CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
public:
    /**
     * @brief Конструктор SqlQuery
     * @param имя_соединения
     */
    Q_INVOKABLE SqlQuery(const QString &connectionName);

    /**
     * @brief Конструктор SqlQuery
     * @param база_данных
     */
    Q_INVOKABLE SqlQuery(SqlDatabase *db);

    /**
     * @brief Деструктор SqlQuery
     */
    virtual ~SqlQuery();

    /**
     * @brief Добавляет значение для привязки
     * @param значение
     */
    Q_INVOKABLE void addBindValue(const QVariant &val);

    /**
     * @brief Возвращает текущую позицию
     * @return
     */
    int at() const;

    /**
     * @brief Привязывает значение к плейсхолдеру или позиции
     * @param плейсхолдер
     * @param значение
     */
    Q_INVOKABLE void bindValue(const QString &placeholder, const QVariant &val);

    /**
     * @brief Привязывает значение к позиции
     * @param позиция
     * @param значение
     */
    Q_INVOKABLE void bindValue(int pos, const QVariant &val);

    /**
     * @brief Очищает запрос
     */
    Q_INVOKABLE void clear();

    /**
     * @brief Выполняет запрос
     * @param запрос
     * @return
     */
    Q_INVOKABLE bool exec(const QString &query = QString());

    /**
     * @brief Возвращает запрос
     * @return
     */
    QSqlQuery *query();

    /**
     * @brief Возвращает выполненный запрос
     * @return
     */
    QString executedQuery() const;

    /**
     * @brief Возвращает последний запрос
     * @return
     */
    QString lastQuery() const;

    /**
     * @brief Возвращает последнее сообщение об ошибке
     * @return
     */
    QString lastError() const;

    /**
     * @brief Возвращает количество затронутых строк
     * @return
     */
    int numRowsAffected() const;

    /**
     * @brief Возвращает количество строк в результате
     * @return
     */
    int size() const;

    /**
     * @brief Проверяет, активен ли запрос
     * @return
     */
    bool isActive() const;

    /**
     * @brief Проверяет, является ли запрос запросом на выборку данных
     * @return
     */
    bool isSelect() const;

    /**
     * @brief Проверяет, валиден ли запрос
     * @return
     */
    bool isValid() const;

    /**
     * @brief Завершает запрос
     */
    Q_INVOKABLE void finish();

    /**
     * @brief Переходит к первой строке результата
     * @return
     */
    Q_INVOKABLE bool first();

    /**
     * @brief Переходит к последней строке результата
     * @return
     */
    Q_INVOKABLE bool last();

    /**
     * @brief Переходит к следующей строке результата
     * @return
     */
    Q_INVOKABLE bool next();

    /**
     * @brief Переходит к предыдущей строке результата
     * @return
     */
    Q_INVOKABLE bool previous();

    /**
     * @brief Проверяет, является ли значение NULL
     * @param поле
     * @return
     */
    Q_INVOKABLE bool isNull(int field) const;

    /**
     * @brief Проверяет, является ли значение NULL
     * @param имя
     * @return
     */
    Q_INVOKABLE bool isNull(const QString &name) const;

    /**
     * @brief Подготавливает запрос
     * @param запрос
     * @return
     */
    Q_INVOKABLE bool prepare(const QString &query);

    /**
     * @brief Переходит к указанной позиции
     * @param индекс
     * @param относительный
     * @return
     */
    Q_INVOKABLE bool seek(int index, bool relative = false);

    /**
     * @brief Возвращает значение поля
     * @param индекс
     * @return
     */
    Q_INVOKABLE QVariant value(int index) const;

    /**
     * @brief Возвращает значение поля
     * @param имя
     * @return
     */
    Q_INVOKABLE QVariant value(const QString &name) const;

private:
    QScopedPointer<QSqlQuery> m_Query;
};
;

#endif // SQLQUERY_H
