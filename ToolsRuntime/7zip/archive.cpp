#include "toolsruntime.h"
#include <QProcess>
#include <QTemporaryDir>
#include <QDirIterator>

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
