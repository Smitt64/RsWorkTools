#include "toolsruntime.h"
#include <QProcess>
#include <QTemporaryDir>
#include <QDirIterator>
#include <QStandardItemModel>
#include <QFileIconProvider>

bool toolCopyDirectory(const QString &sourceDir, const QString &destinationDir)
{
    QDir source(sourceDir);
    QDir destination(destinationDir);

    // Проверяем, существует ли исходный каталог
    if (!source.exists())
        return false;

    // Создаем целевую директорию, если она не существует
    if (!destination.exists())
    {
        if (!destination.mkpath("."))
            return false;
    }

    // Рекурсивно обходим все файлы и подкаталоги
    QDirIterator it(sourceDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        QFileInfo fileInfo = it.fileInfo();
        QString relativePath = source.relativeFilePath(fileInfo.filePath());
        QString destinationPath = destination.filePath(relativePath);

        if (fileInfo.isDir())
        {
            // Если это директория, создаем её в целевой директории
            if (!destination.mkpath(destinationPath))
                return false;
        }
        else if (fileInfo.isFile())
        {
            // Если это файл, копируем его
            if (!QFile::copy(fileInfo.filePath(), destinationPath))
                return false;
        }
    }

    return true;
}

QString extract7zrFromResources(QTemporaryDir &tempDir)
{
    if (!tempDir.isValid()) {
        qWarning() << "Не удалось создать временный каталог.";
        return QString();
    }

    // Путь к 7zr.exe в ресурсах
    QString resourcePath = "://7z";

    // Путь для сохранения во временном каталоге
    QString tempFilePath = tempDir.filePath("7zr.exe");

    // Копируем файл из ресурсов во временный каталог
    if (QFile::copy(resourcePath, tempFilePath)) {
        qInfo() << "7zr.exe успешно извлечен во временный каталог:" << tempFilePath;

        // Устанавливаем права на выполнение (только для Unix-систем)
#ifdef Q_OS_UNIX
        QFile::setPermissions(tempFilePath, QFile::ExeOwner | QFile::ReadOwner | QFile::WriteOwner);
#endif

        return tempFilePath;
    }

    return QString();
}

QStringList collectFiles(const QString &dir)
{
    QStringList files;
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        if (it.fileInfo().isFile())
            files << it.filePath();
    }

    return files;
}

int toolCreateArchive(const QString &archiveName, const QStringList &files)
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return -1;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return -1; // Ошибка извлечения

    QProcess process;
    QStringList args;
    args << "a"; // Команда "добавить в архив"
    args << archiveName; // Имя архива
    args << files; // Список файлов для добавления

    // Запуск процесса с ожиданием завершения
    int result = toolStartProcess(&process, exePath, args, true, true, 30000, false);

    if (result != 0)
        qWarning() << "Ошибка при создании архива. Код ошибки:" << result;

    return result;
}

int toolCreateArchive(const QString &archiveName, const QString &sourceDir)
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return -1;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return -1; // Ошибка извлечения

    // Собираем список файлов с сохранением относительных путей
    QStringList files = collectFiles(sourceDir);

    QProcess process;
    QStringList args;
    args << "a"; // Команда "добавить в архив"
    args << archiveName; // Имя архива
    args << files; // Список файлов для добавления

    // Указываем базовый каталог для сохранения относительных путей
    args << "-r"; // Рекурсивное добавление
    args << "-ssc"; // Сохранять регистр имен файлов
    args << "-spf"; // Использовать полные пути
    args << "-i!" + sourceDir + "/*"; // Указываем базовый каталог

    // Запуск процесса с ожиданием завершения
    int result = toolStartProcess(&process, exePath, args, true, true, 30000, false);

    if (result != 0) {
        qWarning() << "Ошибка при создании архива. Код ошибки:" << result;
    }

    return result;
}

int toolExtractArchive(const QString &archiveName, const QString &extractDir)
{
    // Создаем временный каталог
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return -1;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return -1; // Ошибка извлечения

    QProcess process;
    QStringList args;
    args << "x"; // Команда "извлечь"
    args << archiveName; // Имя архива
    args << "-o" + extractDir; // Путь для извлечения

    // Запуск процесса с ожиданием завершения
    int result = toolStartProcess(&process, exePath, args, true, true, 30000, false);

    if (result != 0)
        qWarning() << "Ошибка при распаковке архива. Код ошибки:" << result;

    return result;
}

int toolExtractDirFromArchive(const QString &archiveName, const QString &targetDir, const QString &dirInArchive)
{
    // Создаем временный каталог
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return -1;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return -1; // Ошибка извлечения

    QProcess process;
    QStringList args;
    args << "x"; // Команда "извлечь"
    args << archiveName; // Имя архива
    args << "-o" + targetDir; // Путь для извлечения
    args << dirInArchive + "/*"; // Путь к каталогу внутри архива

    // Запуск процесса с ожиданием завершения
    int result = toolStartProcess(&process, exePath, args, true, true, 30000, false);

    if (result != 0)
        qWarning() << "Ошибка при распаковке каталога из архива. Код ошибки:" << result;

    return result;
}

QStringList toolGetArchiveFileList(const QString &archivePath)
{
    QStringList fileList;

    // Создаем временный каталог
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return fileList;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return fileList;

    // Запускаем 7zr.exe для получения списка файлов
    QProcess process;
    process.start(exePath, {"l", archivePath});
    if (!process.waitForFinished())
        return fileList;

    // Читаем вывод процесса
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    // Разбираем вывод
    bool fileListStarted = false;
    for (const QString &line : lines)
    {
        if (line.startsWith("----"))
        {
            fileListStarted = true;
            continue;
        }

        if (fileListStarted && !line.trimmed().isEmpty())
        {
            // Извлекаем имя файла (первый столбец)
            QString fileName = line.simplified().split(" ").first();
            fileList << fileName;
        }
    }

    return fileList;
}

QList<ArchiveFileInfo> toolGetArchiveFileInfoList(const QString &archivePath)
{
    QList<ArchiveFileInfo> fileInfoList;

    // Создаем временный каталог
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return fileInfoList;

    // Извлекаем 7zr.exe из ресурсов
    QString exePath = extract7zrFromResources(tempDir);
    if (exePath.isEmpty())
        return fileInfoList;

    // Запускаем 7zr.exe для получения списка файлов
    QProcess process;
    process.start(exePath, {"l", archivePath});
    if (!process.waitForFinished())
        return fileInfoList;

    // Читаем вывод процесса
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    // Разбираем вывод
    bool fileListStarted = false;
    for (const QString &line : lines)
    {
        if (line.startsWith("----"))
        {
            fileListStarted = true;
            continue;
        }

        if (fileListStarted && !line.trimmed().isEmpty())
        {
            // Разбиваем строку на части
            QStringList parts = line.simplified().split(" ", Qt::SkipEmptyParts);
            if (parts.size() >= 5)
            {
                ArchiveFileInfo info;
                info.name = parts[5]; // Имя файла
                info.size = parts[3].toLongLong(); // Размер файла
                info.modified = QDateTime::fromString(parts[0] + " " + parts[1], "yyyy-MM-dd HH:mm:ss"); // Дата изменения
                info.isDirectory = parts[2].startsWith("D"); // Является ли директорией
                fileInfoList << info;
            }
        }
    }

    return fileInfoList;
}

QStandardItemModel* toolCreateArchiveModel(const QList<ArchiveFileInfo> &fileInfoList)
{
    QFileIconProvider provider;
    auto *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Имя файла", "Размер", "Дата изменения", "Тип"});

    // Словарь для хранения иерархии
    QMap<QString, QStandardItem*> directoryMap;
    directoryMap[""] = model->invisibleRootItem(); // Корневой элемент

    // Обрабатываем каждый файл
    for (const ArchiveFileInfo &info : fileInfoList)
    {
        QFileInfo fileInfo(info.name);

        // Получаем родительскую директорию
        QString parentDir = fileInfo.path();
        if (!directoryMap.contains(parentDir))
        {
            // Создаем элементы для родительских директорий
            QStringList dirs = parentDir.split("/", Qt::SkipEmptyParts);
            QString currentPath;
            QStandardItem *parentItem = directoryMap[""];

            for (const QString &dir : dirs)
            {
                currentPath += (currentPath.isEmpty() ? "" : "/") + dir;
                if (!directoryMap.contains(currentPath))
                {
                    auto *dirItem = new QStandardItem(dir);
                    dirItem->setIcon(provider.icon(QFileIconProvider::Folder)); // Иконка для директории
                    parentItem->appendRow(dirItem);
                    directoryMap[currentPath] = dirItem;
                }

                parentItem = directoryMap[currentPath];
            }
        }

        // Создаем элемент для файла
        auto *fileItem = new QStandardItem(fileInfo.fileName());
        fileItem->setIcon(provider.icon(QFileIconProvider::File)); // Иконка для файла

        // Добавляем информацию о размере и дате изменения
        auto *sizeItem = new QStandardItem(QString::number(info.size));
        auto *dateItem = new QStandardItem(info.modified.toString("yyyy-MM-dd HH:mm:ss"));
        auto *typeItem = new QStandardItem(info.isDirectory ? "Folder" : "File");

        // Добавляем строку в модель
        QList<QStandardItem*> rowItems = {fileItem, sizeItem, dateItem, typeItem};
        directoryMap[parentDir]->appendRow(rowItems);
    }

    return model;
}
