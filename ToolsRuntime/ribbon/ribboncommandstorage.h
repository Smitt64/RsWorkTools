// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef RIBBONCOMMANDSTORAGE_H
#define RIBBONCOMMANDSTORAGE_H

#include "ToolsRuntime_global.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QJsonArray>

class QAbstractTableModel;

struct TOOLSRUNTIME_EXPORT RibbonCommandItem
{
    RibbonCommandItem();

    QString id;
    QString macrofile;
    QString title;
    QString icon;
};

class RibbonCommandModel;
class TOOLSRUNTIME_EXPORT RibbonCommandStorage : public QObject
{
    Q_OBJECT
public:
    explicit RibbonCommandStorage(QObject *parent = nullptr);
    virtual ~RibbonCommandStorage();

    int count() const;
    RibbonCommandItem *command(int index);
    const RibbonCommandItem *command(int index) const;

    void addCommand(const RibbonCommandItem &item);
    void addCommand(const QString &macrofile, const QString &title);
    void removeCommand(int index);

    void setTitle(int index, const QString &title);
    void setIcon(int index, const QString &icon);

    QAbstractTableModel *model() const;

    QJsonArray save() const;
    void load(const QJsonArray &data);

signals:
    void commandListChanged();

private:
    QList<RibbonCommandItem> m_commands;
    RibbonCommandModel *m_model;
};

#endif // RIBBONCOMMANDSTORAGE_H
