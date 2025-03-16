#ifndef IOFILEINFO_H
#define IOFILEINFO_H

#include <QObject>
#include <QDateTime>

class QFileInfo;
class IoFileInfo : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE IoFileInfo(const QString& path, QObject *parent = nullptr);

    /**
     * @brief Проверяет, существует ли файл или директория.
     * @return true, если файл или директория существует, иначе false.
     */
    Q_INVOKABLE bool exists() const;

    /**
     * @brief Возвращает абсолютный путь к файлу.
     * @return Абсолютный путь.
     */
    Q_INVOKABLE QString absoluteFilePath() const;

    /**
     * @brief Возвращает имя файла.
     * @return Имя файла.
     */
    Q_INVOKABLE QString fileName() const;

    /**
     * @brief Возвращает базовое имя файла (без расширения).
     * @return Базовое имя файла.
     */
    Q_INVOKABLE QString baseName() const;

    /**
     * @brief Возвращает расширение файла.
     * @return Расширение файла.
     */
    Q_INVOKABLE QString suffix() const;

    /**
     * @brief Возвращает путь к директории.
     * @return Путь к директории.
     */
    Q_INVOKABLE QString path() const;

    /**
     * @brief Возвращает абсолютный путь к директории.
     * @return Абсолютный путь к директории.
     */
    Q_INVOKABLE QString absolutePath() const;

    /**
     * @brief Проверяет, является ли объект файлом.
     * @return true, если это файл, иначе false.
     */
    Q_INVOKABLE bool isFile() const;

    /**
     * @brief Проверяет, является ли объект директорией.
     * @return true, если это директория, иначе false.
     */
    Q_INVOKABLE bool isDir() const;

    /**
     * @brief Возвращает размер файла.
     * @return Размер файла в байтах.
     */
    Q_INVOKABLE qint64 size() const;

    /**
     * @brief Возвращает время последнего изменения.
     * @return Время последнего изменения.
     */
    Q_INVOKABLE QDateTime lastModified() const;

    /**
     * @brief Возвращает время последнего чтения.
     * @return Время последнего чтения.
     */
    Q_INVOKABLE QDateTime lastRead() const;

    /**
     * @brief Возвращает время создания.
     * @return Время создания.
     */
    Q_INVOKABLE QDateTime created() const;

    /**
     * @brief Проверяет, доступен ли файл для чтения.
     * @return true, если файл доступен для чтения, иначе false.
     */
    Q_INVOKABLE bool isReadable() const;

    /**
     * @brief Проверяет, доступен ли файл для записи.
     * @return true, если файл доступен для записи, иначе false.
     */
    Q_INVOKABLE bool isWritable() const;

    /**
     * @brief Проверяет, доступен ли файл для выполнения.
     * @return true, если файл доступен для выполнения, иначе false.
     */
    Q_INVOKABLE bool isExecutable() const;

private:
    QScopedPointer<QFileInfo> fileInfo;
};

#endif // IOFILEINFO_H
