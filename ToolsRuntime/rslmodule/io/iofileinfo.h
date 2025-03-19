#ifndef IOFILEINFO_H
#define IOFILEINFO_H

#include <QObject>
#include <QDateTime>

class QFileInfo;
class IoFileInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool exists READ exists CONSTANT)
    Q_PROPERTY(QString absoluteFilePath READ absoluteFilePath CONSTANT)
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(QString baseName READ baseName CONSTANT)
    Q_PROPERTY(QString suffix READ suffix CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(QString absolutePath READ absolutePath CONSTANT)
    Q_PROPERTY(bool isFile READ isFile CONSTANT)
    Q_PROPERTY(bool isDir READ isDir CONSTANT)
    Q_PROPERTY(qint64 size READ size CONSTANT)
    Q_PROPERTY(QDateTime lastModified READ lastModified CONSTANT)
    Q_PROPERTY(QDateTime lastRead READ lastRead CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(bool isReadable READ isReadable CONSTANT)
    Q_PROPERTY(bool isWritable READ isWritable CONSTANT)
    Q_PROPERTY(bool isExecutable READ isExecutable CONSTANT)
public:
    Q_INVOKABLE IoFileInfo(const QString& path, QObject *parent = nullptr);

    /**
     * @brief Проверяет, существует ли файл или директория.
     * @return true, если файл или директория существует, иначе false.
     */
    bool exists() const;

    /**
     * @brief Возвращает абсолютный путь к файлу.
     * @return Абсолютный путь.
     */
    QString absoluteFilePath() const;

    /**
     * @brief Возвращает имя файла.
     * @return Имя файла.
     */
    QString fileName() const;

    /**
     * @brief Возвращает базовое имя файла (без расширения).
     * @return Базовое имя файла.
     */
    QString baseName() const;

    /**
     * @brief Возвращает расширение файла.
     * @return Расширение файла.
     */
    QString suffix() const;

    /**
     * @brief Возвращает путь к директории.
     * @return Путь к директории.
     */
    QString path() const;

    /**
     * @brief Возвращает абсолютный путь к директории.
     * @return Абсолютный путь к директории.
     */
    QString absolutePath() const;

    /**
     * @brief Проверяет, является ли объект файлом.
     * @return true, если это файл, иначе false.
     */
    bool isFile() const;

    /**
     * @brief Проверяет, является ли объект директорией.
     * @return true, если это директория, иначе false.
     */
    bool isDir() const;

    /**
     * @brief Возвращает размер файла.
     * @return Размер файла в байтах.
     */
    qint64 size() const;

    /**
     * @brief Возвращает время последнего изменения.
     * @return Время последнего изменения.
     */
    QDateTime lastModified() const;

    /**
     * @brief Возвращает время последнего чтения.
     * @return Время последнего чтения.
     */
    QDateTime lastRead() const;

    /**
     * @brief Возвращает время создания.
     * @return Время создания.
     */
    QDateTime created() const;

    /**
     * @brief Проверяет, доступен ли файл для чтения.
     * @return true, если файл доступен для чтения, иначе false.
     */
    bool isReadable() const;

    /**
     * @brief Проверяет, доступен ли файл для записи.
     * @return true, если файл доступен для записи, иначе false.
     */
    bool isWritable() const;

    /**
     * @brief Проверяет, доступен ли файл для выполнения.
     * @return true, если файл доступен для выполнения, иначе false.
     */
    bool isExecutable() const;

private:
    QScopedPointer<QFileInfo> fileInfo;
};

#endif // IOFILEINFO_H
