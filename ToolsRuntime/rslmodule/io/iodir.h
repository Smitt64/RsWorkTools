#ifndef IODIR_H
#define IODIR_H

#include <QObject>
#include <QDir>

class IoDir : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE IoDir(const QString& path = QString());

    /**
     * @brief Проверяет, существует ли директория.
     * @return true, если директория существует, иначе false.
     */
    Q_INVOKABLE bool exists() const;

    /**
     * @brief Возвращает абсолютный путь к директории.
     * @return Абсолютный путь.
     */
    Q_INVOKABLE QString absolutePath() const;

    /**
     * @brief Возвращает канонический путь (без символических ссылок).
     * @return Канонический путь.
     */
    Q_INVOKABLE QString canonicalPath() const;

    /**
     * @brief Возвращает имя директории.
     * @return Имя директории.
     */
    Q_INVOKABLE QString dirName() const;

    /**
     * @brief Возвращает список файлов и директорий.
     * @param filters Фильтры для отбора файлов и директорий.
     * @param filterFlags Флаги фильтрации.
     * @param sortFlags Флаги сортировки.
     * @return Список файлов и директорий.
     */
    Q_INVOKABLE QStringList entryList(const QStringList& filters = QStringList(), QDir::Filters filterFlags = QDir::NoFilter, QDir::SortFlags sortFlags = QDir::NoSort) const;

    /**
     * @brief Возвращает список файлов.
     * @param filters Фильтры для отбора файлов.
     * @param filterFlags Флаги фильтрации.
     * @param sortFlags Флаги сортировки.
     * @return Список файлов.
     */
    Q_INVOKABLE QStringList files(const QStringList& filters = QStringList(), QDir::Filters filterFlags = QDir::NoFilter, QDir::SortFlags sortFlags = QDir::NoSort) const;

    /**
     * @brief Возвращает список директорий.
     * @param filters Фильтры для отбора директорий.
     * @param filterFlags Флаги фильтрации.
     * @param sortFlags Флаги сортировки.
     * @return Список директорий.
     */
    Q_INVOKABLE QStringList directories(const QStringList& filters = QStringList(), QDir::Filters filterFlags = QDir::NoFilter, QDir::SortFlags sortFlags = QDir::NoSort) const;

    /**
     * @brief Возвращает список символических ссылок.
     * @param filters Фильтры для отбора символических ссылок.
     * @param sortFlags Флаги сортировки.
     * @return Список символических ссылок.
     */
    Q_INVOKABLE QStringList symLinks(const QStringList& filters = QStringList(), QDir::SortFlags sortFlags = QDir::NoSort) const;

    /**
     * @brief Создает директорию.
     * @param dirName Имя новой директории.
     * @return true, если директория создана, иначе false.
     */
    Q_INVOKABLE bool mkdir(const QString& dirName) const;

    /**
     * @brief Создает все родительские директории.
     * @param dirPath Путь к директории.
     * @return true, если все директории созданы, иначе false.
     */
    Q_INVOKABLE bool mkpath(const QString& dirPath) const;

    /**
     * @brief Удаляет директорию.
     * @param dirName Имя директории.
     * @return true, если директория удалена, иначе false.
     */
    Q_INVOKABLE bool rmdir(const QString& dirName) const;

    /**
     * @brief Удаляет директорию и все её содержимое.
     * @return true, если директория удалена, иначе false.
     */
    Q_INVOKABLE bool removeRecursively() const;

    /**
     * @brief Переименовывает директорию.
     * @param oldName Старое имя директории.
     * @param newName Новое имя директории.
     * @return true, если директория переименована, иначе false.
     */
    Q_INVOKABLE bool rename(const QString& oldName, const QString& newName);

    /**
     * @brief Переходит в родительскую директорию.
     * @return true, если переход выполнен, иначе false.
     */
    Q_INVOKABLE bool cdUp();

    /**
     * @brief Переходит в указанную директорию.
     * @param dirName Имя директории.
     * @return true, если переход выполнен, иначе false.
     */
    Q_INVOKABLE bool cd(const QString& dirName);

    /**
     * @brief Возвращает текущую директорию.
     * @return Текущая директория.
     */
    Q_INVOKABLE QString currentPath() const;

    /**
     * @brief Очищает директорию (удаляет все файлы и поддиректории).
     * @return true, если директория очищена, иначе false.
     */
    Q_INVOKABLE bool clear();

    /**
     * @brief Возвращает путь к файлу или директории.
     * @param fileName Имя файла или директории.
     * @return Путь к файлу или директории.
     */
    Q_INVOKABLE QString filePath(const QString& fileName) const;

    /**
     * @brief Возвращает абсолютный путь к файлу или директории.
     * @param fileName Имя файла или директории.
     * @return Абсолютный путь.
     */
    Q_INVOKABLE QString absoluteFilePath(const QString& fileName) const;

    /**
     * @brief Проверяет, является ли путь относительным.
     * @return true, если путь относительный, иначе false.
     */
    Q_INVOKABLE bool isRelative() const;

    /**
     * @brief Преобразует относительный путь в абсолютный.
     * @param relativePath Относительный путь.
     * @return Абсолютный путь.
     */
    Q_INVOKABLE QString absoluteFilePathFromRelative(const QString& relativePath) const;

    /**
     * @brief Возвращает корневую директорию.
     * @return Корневая директория.
     */
    QString rootPath() const;

    /**
     * @brief Проверяет, является ли путь корневым.
     * @return true, если путь корневой, иначе false.
     */
    bool isRoot() const;

    /**
     * @brief Возвращает домашнюю директорию пользователя.
     * @return Домашняя директория.
     */
    QString homePath() const;

    /**
     * @brief Возвращает временную директорию.
     * @return Временная директория.
     */
    QString tempPath() const;

    /**
     * @brief Возвращает количество файлов и директорий.
     * @return Количество файлов и директорий.
     */
    int count() const;

    /**
     * @brief Возвращает информацию о файле или директории.
     * @param fileName Имя файла или директории.
     * @return Информация о файле или директории.
     */
    QObject *fileInfo(const QString& fileName) const;

private:
    QScopedPointer<QDir> dir;
};

#endif // IODIR_H
