#ifndef IONAMESPACE_H
#define IONAMESPACE_H

#include <QObject>

class IoNamespace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath CONSTANT)
    Q_PROPERTY(QString home READ home CONSTANT)
    Q_PROPERTY(QString root READ root CONSTANT)
    Q_PROPERTY(QString temp READ temp CONSTANT)
    Q_PROPERTY(QString listSeparator READ listSeparator CONSTANT)
    Q_PROPERTY(QString separator READ separator CONSTANT)
    Q_PROPERTY(QStringList drives READ drives CONSTANT)
public:
    IoNamespace(QObject *parent = nullptr);
    virtual ~IoNamespace() = default;

    QString currentPath();
    QString home();
    QString root();
    QString temp();
    QString listSeparator();
    QString separator();
    QStringList drives();

    Q_INVOKABLE QString cleanPath(const QString &path);
    Q_INVOKABLE QString fromNativeSeparators(const QString &pathName);
    Q_INVOKABLE bool isAbsolutePath(const QString &path);
    Q_INVOKABLE bool isRelativePath(const QString &path);
    Q_INVOKABLE bool match(const QString &filter, const QString &fileName);
    Q_INVOKABLE bool match(const QStringList &filters, const QString &fileName);
    Q_INVOKABLE QString toNativeSeparators(const QString &pathName);

    Q_INVOKABLE bool copyFile(const QString &fileName, const QString &newName);
    Q_INVOKABLE bool existsFile(const QString &fileName);
    Q_INVOKABLE bool moveToTrash(const QString &fileName);
    Q_INVOKABLE bool removeFile(const QString &fileName);
    Q_INVOKABLE bool renameFile(const QString &oldName, const QString &newName);

    Q_INVOKABLE QStringList entryList(const QString &path, 
        const QStringList &nameFilters, 
        int filters = 0, int sort = -1);
    Q_INVOKABLE QStringList entryList(const QString &path, 
        int filters = 0, int sort = -1);
};

#endif // IONAMESPACE_H
