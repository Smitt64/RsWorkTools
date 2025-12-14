#ifndef ICONTHEMEMANAGER_H
#define ICONTHEMEMANAGER_H

#include <QString>
#include <QStringList>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT IconThemeManager
{
public:
    // Основной метод инициализации темы иконок
    static void initialize(const QString& themeName = "vs_theme");

    // Добавление пользовательского пути поиска иконок
    static void addCustomPath(const QString& path);

    // Получение текущих путей поиска иконок
    static QStringList getIconSearchPaths();

    // Проверка доступности иконки в теме
    static bool hasIcon(const QString& iconName);

    // Установка имени темы
    static void setThemeName(const QString& themeName);

    // Получение текущего имени темы
    static QString themeName();

    // Получение списка всех доступных путей (для отладки)
    static QStringList currentSearchPaths();

private:
    // Вспомогательный метод для добавления пути с проверкой существования
    static void addIfExists(QStringList& list, const QString& path);

    // Логирование информации для отладки
    static void logDebugInfo();

    // Вспомогательный метод для получения базовых путей приложения
    static QStringList getApplicationIconPaths();

    // Вспомогательный метод для получения путей рабочего каталога
    static QStringList getWorkingDirectoryIconPaths();

    // Вспомогательный метод для получения системных путей
    static QStringList getSystemIconPaths();
};

#endif // ICONTHEMEMANAGER_H
