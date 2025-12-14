#include "IconThemeManager.h"
#include <QIcon>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

// Основной метод инициализации темы иконок
void IconThemeManager::initialize(const QString& themeName)
{
    QStringList paths = getIconSearchPaths();
    QIcon::setThemeSearchPaths(paths);
    QIcon::setThemeName(themeName);

    logDebugInfo();
}

// Добавление пользовательского пути поиска иконок
void IconThemeManager::addCustomPath(const QString& path)
{
    QDir dir(path);
    if (dir.exists()) {
        QString cleanPath = QDir::cleanPath(path);
        QStringList paths = QIcon::themeSearchPaths();
        if (!paths.contains(cleanPath)) {
            paths.prepend(cleanPath); // Высокий приоритет
            QIcon::setThemeSearchPaths(paths);

#ifdef QT_DEBUG
            qDebug() << "Added custom icon path:" << cleanPath;
#endif
        }
    }
}

// Получение текущих путей поиска иконок
QStringList IconThemeManager::getIconSearchPaths()
{
    QStringList paths;

    // 1. Пути приложения (высший приоритет)
    paths.append(getApplicationIconPaths());

    // 2. Пути рабочего каталога
    paths.append(getWorkingDirectoryIconPaths());

    // 3. Системные пути (низший приоритет)
    paths.append(getSystemIconPaths());

    // Удаляем дубликаты
    QStringList uniquePaths;
    for (const QString& path : paths) {
        if (!uniquePaths.contains(path)) {
            uniquePaths.append(path);
        }
    }

    return uniquePaths;
}

// Проверка доступности иконки в теме
bool IconThemeManager::hasIcon(const QString& iconName)
{
    QIcon icon = QIcon::fromTheme(iconName);
    return !icon.isNull();
}

// Установка имени темы
void IconThemeManager::setThemeName(const QString& themeName)
{
    QIcon::setThemeName(themeName);

#ifdef QT_DEBUG
    qDebug() << "Icon theme set to:" << themeName;
#endif
}

// Получение текущего имени темы
QString IconThemeManager::themeName()
{
    return QIcon::themeName();
}

// Получение списка всех доступных путей (для отладки)
QStringList IconThemeManager::currentSearchPaths()
{
    return QIcon::themeSearchPaths();
}

// Вспомогательный метод для добавления пути с проверкой существования
void IconThemeManager::addIfExists(QStringList& list, const QString& path)
{
    QString cleanPath = QDir::cleanPath(path);
    QDir dir(cleanPath);
    if (dir.exists() && !list.contains(cleanPath)) {
        list.append(cleanPath);
    }
}

// Логирование информации для отладки
void IconThemeManager::logDebugInfo()
{
#ifdef QT_DEBUG
    qDebug() << "=== Icon Theme Initialization ===";
    qDebug() << "Application directory:" << QCoreApplication::applicationDirPath();
    qDebug() << "Current directory:" << QDir::currentPath();
    qDebug() << "Theme name:" << QIcon::themeName();

    QStringList paths = QIcon::themeSearchPaths();
    qDebug() << "Search paths (" << paths.size() << "):";

    for (int i = 0; i < paths.size(); ++i) {
        QDir dir(paths[i]);
        QString status = dir.exists() ? "✓ exists" : "✗ missing";
        qDebug() << QString("  [%1] %2 (%3)")
                        .arg(i + 1, 2)
                        .arg(paths[i])
                        .arg(status);
    }

    // Проверка доступности некоторых стандартных иконок
    QStringList testIcons = {"document-open", "edit-copy", "edit-paste", "application-exit"};
    qDebug() << "Icon availability test:";
    for (const QString& iconName : testIcons) {
        bool available = hasIcon(iconName);
        qDebug() << QString("  %1: %2")
                        .arg(iconName, -20)
                        .arg(available ? "✓ available" : "✗ not found");
    }

    qDebug() << "================================";
#endif
}

// Вспомогательный метод для получения базовых путей приложения
QStringList IconThemeManager::getApplicationIconPaths()
{
    QStringList paths;
    QString appDir = QCoreApplication::applicationDirPath();

    // Основные пути относительно каталога приложения
    QStringList appRelativePaths = {
        "/icons",
        "/resources/icons",
        "/../icons",
        "/../resources/icons",
        "/share/icons",
        "/../share/icons"
    };

    for (const QString& relativePath : appRelativePaths) {
        QString fullPath = QDir::cleanPath(appDir + relativePath);
        addIfExists(paths, fullPath);
    }

    return paths;
}

// Вспомогательный метод для получения путей рабочего каталога
QStringList IconThemeManager::getWorkingDirectoryIconPaths()
{
    QStringList paths;
    QString currentDir = QDir::currentPath();
    QString appDir = QCoreApplication::applicationDirPath();

    // Добавляем пути только если рабочий каталог отличается от каталога приложения
    if (currentDir != appDir) {
        QStringList currentRelativePaths = {
            "/icons",
            "/../icons",
            "/resources/icons",
            "/../resources/icons"
        };

        for (const QString& relativePath : currentRelativePaths) {
            QString fullPath = QDir::cleanPath(currentDir + relativePath);
            addIfExists(paths, fullPath);
        }
    }

    return paths;
}

// Вспомогательный метод для получения системных путей
QStringList IconThemeManager::getSystemIconPaths()
{
    QStringList paths;

    // Стандартные системные пути
    QStringList standardPaths = QStandardPaths::standardLocations(
        QStandardPaths::GenericDataLocation
        );

    for (const QString& path : standardPaths) {
        // path/icons
        QString iconsPath = QDir::cleanPath(path + "/icons");
        addIfExists(paths, iconsPath);

// Для Unix-like систем
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
        QStringList unixPaths = {
            "/../share/icons",
            "/../local/share/icons",
            "/usr/share/icons",
            "/usr/local/share/icons"
        };

        for (const QString& unixPath : unixPaths) {
            QString fullPath = QDir::cleanPath(path + unixPath);
            addIfExists(paths, fullPath);
        }
#endif

// Для Windows
#ifdef Q_OS_WIN
        QStringList windowsPaths = {
            "/../Microsoft/Windows/Themes/icons",
            "/../icons"
        };

        for (const QString& winPath : windowsPaths) {
            QString fullPath = QDir::cleanPath(path + winPath);
            addIfExists(paths, fullPath);
        }
#endif
    }

    return paths;
}
