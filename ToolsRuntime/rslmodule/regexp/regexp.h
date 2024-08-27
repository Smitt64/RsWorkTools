#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>

class QRegExp;
/**
 * @brief Класс для работы с регулярными выражениями
 */
class RegExp : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор RegExp
     * @param шаблон
     */
    Q_INVOKABLE RegExp(const QString &_pattern);

    /**
     * @brief Проверяет, пустое ли регулярное выражение
     * @return
     */
    Q_INVOKABLE bool isEmpty() const;

    /**
     * @brief Проверяет, валидно ли регулярное выражение
     * @return
     */
    Q_INVOKABLE bool isValid() const;

    /**
     * @brief Возвращает шаблон регулярного выражения
     * @return
     */
    Q_INVOKABLE QString pattern() const;

    /**
     * @brief Устанавливает шаблон регулярного выражения
     * @param шаблон
     */
    Q_INVOKABLE void setPattern(const QString &pattern);

    /**
     * @brief Проверяет, минимально ли регулярное выражение
     * @return
     */
    Q_INVOKABLE bool isMinimal() const;

    /**
     * @brief Устанавливает минимальность регулярного выражения
     * @param минимальность
     */
    Q_INVOKABLE void setMinimal(bool minimal);

    /**
     * @brief Проверяет, полностью ли соответствует строка шаблону регулярного выражения
     * @param строка
     * @return
     */
    Q_INVOKABLE bool exactMatch(const QString &str) const;

    /**
     * @brief Возвращает индекс первого вхождения шаблона в строку
     * @param строка
     * @param смещение
     * @return
     */
    Q_INVOKABLE int indexIn(const QString &str, int offset = 0) const;

    /**
     * @brief Возвращает индекс последнего вхождения шаблона в строку
     * @param строка
     * @param смещение
     * @return
     */
    Q_INVOKABLE int lastIndexIn(const QString &str, int offset = -1) const;

    /**
     * @brief Возвращает длину совпадающей части шаблона и строки
     * @return
     */
    Q_INVOKABLE int matchedLength() const;

    /**
     * @brief Возвращает количество групп в регулярном выражении
     * @return
     */
    Q_INVOKABLE int captureCount() const;

    /**
     * @brief Возвращает список совпадающих строк
     * @return
     */
    Q_INVOKABLE QStringList capturedTexts() const;

    /**
     * @brief Возвращает список совпадающих строк
     * @return
     */
    QStringList capturedTexts();

    /**
     * @brief Возвращает совпадающую строку по номеру группы
     * @param номер
     * @return
     */
    Q_INVOKABLE QString cap(int nth = 0) const;

    /**
     * @brief Возвращает совпадающую строку по номеру группы
     * @param номер
     * @return
     */
    QString cap(int nth = 0);

    /**
     * @brief Возвращает позицию совпадающей части шаблона и строки
     * @param номер
     * @return
     */
    Q_INVOKABLE int pos(int nth = 0) const;

    /**
     * @brief Возвращает позицию совпадающей части шаблона и строки
     * @param номер
     * @return
     */
    int pos(int nth = 0);

    /**
     * @brief Возвращает строку с описанием последней ошибки
     * @return
     */
    Q_INVOKABLE QString errorString() const;

    /**
     * @brief Возвращает строку с описанием последней ошибки
     * @return
     */
    QString errorString();

private:
    QScopedPointer<QRegExp> m_pRegExp;
};
;

/**
 * @brief Проверяет, соответствует ли строка шаблону регулярного выражения
 * @param шаблон
 * @param строка
 * @param позиция
 * @return
 */
bool RegExpValidate(const QString &patern, const QString &str, int &pos);


#endif // REGEXP_H
