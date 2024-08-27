// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ERRORSMODEL_H
#define ERRORSMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QDateTime>
#include "ToolsRuntime_global.h"

class ErrorsModelPrivate;
/**
 * @brief Модель данных для отображения ошибок и предупреждений
 */
class TOOLSRUNTIME_EXPORT ErrorsModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasErrors READ hasErrors)
    Q_PROPERTY(bool hasWarnings READ hasWarnings)
    Q_PROPERTY(bool isEmpty READ isEmpty)

    Q_PROPERTY(int errorsCount READ errorsCount)
    Q_PROPERTY(int warningsCount READ warningsCount)
    Q_PROPERTY(int infoCount READ infoCount)
public:
        enum ErrorType 
    { 
        TypeError = 0, 
        TypeWarning, 
        TypeInfo 
    }; 
    Q_ENUMS(ErrorType) 
 
    /**
     * @brief Конструктор ErrorsModel
     * @param родитель
     */
    Q_INVOKABLE ErrorsModel(QObject *parent = nullptr);

    /**
     * @brief Деструктор ErrorsModel
     */
    virtual ~ErrorsModel();

    /**
     * @brief Возвращает количество столбцов в модели
     * @param родитель
     * @return
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Возвращает данные по указанному индексу
     * @param индекс
     * @param роль
     * @return
     */
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Возвращает индекс строки и столбца
     * @param строка
     * @param столбец
     * @param родитель
     * @return
     */
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Возвращает родительский индекс
     * @param индекс
     * @return
     */
    virtual QModelIndex parent (const QModelIndex &index) const;

    /**
     * @brief Возвращает количество строк в модели
     * @param родитель
     * @return
     */
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Возвращает флаги для указанного индекса
     * @param индекс
     * @return
     */
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * @brief Устанавливает данные по указанному индексу
     * @param индекс
     * @param значение
     * @param роль
     * @return
     */
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /**
     * @brief Возвращает заголовок для указанного столбца
     * @param секция
     * @param ориентация
     * @param роль
     * @return
     */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
     * @brief Проверяет наличие ошибок
     * @return
     */
    bool hasErrors() const;

    /**
     * @brief Проверяет наличие предупреждений
     * @return
     */
    bool hasWarnings() const;

    /**
     * @brief Проверяет, пуста ли модель
     * @return
     */
    bool isEmpty() const;

    /**
     * @brief Возвращает количество ошибок
     * @return
     */
    int errorsCount() const;

    /**
     * @brief Возвращает количество предупреждений
     * @return
     */
    int warningsCount() const;

    /**
     * @brief Возвращает количество информационных сообщений
     * @return
     */
    int infoCount() const;

    /**
     * @brief Добавляет ошибку в модель
     * @param текст
     */
    Q_INVOKABLE void addError(const QString &text);

    /**
     * @brief Добавляет информационное сообщение в модель
     * @param текст
     */
    Q_INVOKABLE void addMessage(const QString &text);

    /**
     * @brief Добавляет ошибку в модель с указанным типом и временем
     * @param текст
     * @param тип
     * @param датаВремя
     */
    Q_INVOKABLE void appendError(const QString &text, const qint16 &type = TypeError, const QDateTime &dateTime = QDateTime());

    /**
     * @brief Добавляет информационное сообщение в модель с указанным временем
     * @param текст
     * @param датаВремя
     */
    Q_INVOKABLE void appendMessage(const QString &text, const QDateTime &dateTime = QDateTime());

    /**
     * @brief Возвращает тип ошибки или предупреждения
     * @param индекс
     * @return
     */
    Q_INVOKABLE qint16 type(const qint32 &row = -1) const;

    /**
     * @brief Возвращает текст ошибки или предупреждения
     * @return
     */
    Q_INVOKABLE QString text() const;

    /**
     * @brief Возвращает текст ошибки или предупреждения по указанному индексу
     * @param индекс
     * @return
     */
    Q_INVOKABLE QString textAt(const int &i) const;

    /**
     * @brief Очищает модель
     */
    Q_INVOKABLE void clear();

    /**
     * @brief Переходит к следующей ошибке или предупреждению
     * @return
     */
    Q_INVOKABLE bool next();

    /**
     * @brief Возвращает к первой ошибке или предупреждению
     * @return
     */
    Q_INVOKABLE bool first();

signals:
    void errorsCountChanged(quint32);
    void warningsCountChanged(quint32);
    void infoCountChanged(quint32);
    void newError(QString);
    void newMessage(QString);

private:
    ErrorsModelPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ErrorsModel);
};
;

#endif // ERRORSMODEL_H
