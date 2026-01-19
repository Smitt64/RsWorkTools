#include "svnstatusmodel.h"
#include "toolsruntime.h"
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>
#include <QRegularExpression>

SvnStatusModel::SvnStatusModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_VcsType(VcsType::None)
{

}

SvnStatusModel::~SvnStatusModel()
{

}

int SvnStatusModel::columnCount(const QModelIndex &parent) const
{
    return fld_Count;
}

QModelIndex SvnStatusModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex SvnStatusModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int SvnStatusModel::rowCount(const QModelIndex &parent) const
{
    return m_Elements.size();
}

QVariant SvnStatusModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == fld_FullFile)
            return m_Elements[index.row()].fullpath;
        else if (index.column() == fld_File)
            return m_Elements[index.row()].path;
        else if (index.column() == fld_Action)
            return m_Elements[index.row()].action;
        else if (index.column() == fld_FileName)
            return m_Elements[index.row()].filename;
    }
    return QVariant();
}

const SvnSatatusElement &SvnStatusModel::element(const int &row) const
{
    return m_Elements[row];
}

void SvnStatusModel::setPath(const QString &path, const QString &revision)
{
    beginResetModel();
    m_Elements.clear();
    m_Path = path;

    VcsType actualVcsType = m_VcsType;

    // Если установлен авторежим, определяем тип VCS автоматически
    if (m_VcsType == VcsType::None)
    {
        actualVcsType = detectVcsType(path);
    }

    if (actualVcsType == VcsType::Svn)
    {
        setPathSvn(path, revision);
    }
    else if (actualVcsType == VcsType::Git)
    {
        setPathGit(path, revision);
    }
    else
    {
        // Если VCS не обнаружена, очищаем модель
        m_Elements.clear();
    }

    endResetModel();
}

void SvnStatusModel::setVcsType(VcsType type)
{
    m_VcsType = type;
}

VcsType SvnStatusModel::detectVcsType(const QString &path)
{
    QDir dir(path);

    // Проверяем наличие каталога .git
    if (dir.exists(".git"))
    {
        return VcsType::Git;
    }

    // Проверяем наличие каталога .svn
    if (dir.exists(".svn"))
    {
        return VcsType::Svn;
    }

    // Рекурсивно проверяем родительские каталоги для Git
    QString currentPath = path;
    while (!currentPath.isEmpty() && QDir(currentPath).exists())
    {
        QDir currentDir(currentPath);
        if (currentDir.exists(".git"))
        {
            return VcsType::Git;
        }

        // Для SVN проверяем только текущий каталог (не рекурсивно)
        if (currentPath == path && currentDir.exists(".svn"))
        {
            return VcsType::Svn;
        }

        // Поднимаемся на уровень выше
        QString parentPath = QDir(currentPath).absolutePath();
        if (parentPath == currentPath) // Достигли корня
            break;

        currentPath = QDir(currentPath).absoluteFilePath("..");
        if (currentPath == parentPath) // Достигли корня
            break;
    }

    return VcsType::None;
}

void SvnStatusModel::setPathSvn(const QString &path, const QString &revision)
{
    SvnInfoMap info = toolSvnGetRepoInfo(path);

    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args = {"--xml"};

    if (!revision.isEmpty())
    {
        args.prepend("log");
        args.append("-r");
        args.append(revision);
        args.append("-v");
        args.append(info["url"]);
    }
    else
    {
        args.prepend("status");
        args.append("--ignore-externals");
    }

    toolStartProcess(&proc, "svn.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QDomDocument doc;
    doc.setContent(data);

    QDomElement docElem = doc.documentElement();
    auto entryFunc = [=](const QDomElement &e)
    {
        if (e.nodeName() != "entry")
            return;

        QDir main(m_Path);
        SvnSatatusElement element;
        element.path = e.attribute("path");
        element.fullpath = main.absoluteFilePath(e.attribute("path"));

        QFileInfo fi(element.fullpath);

        QDomElement wcstatus = e.firstChild().toElement();
        QDomElement commit = wcstatus.firstChild().toElement();

        element.action = wcstatus.attribute("item");
        element.revision = wcstatus.attribute("revision");
        element.props = wcstatus.attribute("props");
        element.filename = fi.completeBaseName();

        element.author = commit.firstChildElement("author").text();
        m_Elements.append(element);
    };

    if (revision.isEmpty())
    {
        QDomNode n = docElem.firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();

            if (e.nodeName() == "target")
            {
                QDomNode target = e.firstChild();

                while(!target.isNull())
                {
                    entryFunc(target.toElement());
                    target = target.nextSibling();
                }
            }
            n = n.nextSibling();
        }
    }
    else
    {
        QDomNode n = docElem.firstChildElement("logentry").firstChildElement("paths").firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();

            if (e.nodeName() == "path")
            {
                SvnSatatusElement element;

                QString rel_url = info["relative-url"].mid(2) + "/";
                element.fullpath = e.text().mid(1);
                element.path = e.text().remove(rel_url).mid(1);

                QFileInfo fi(element.fullpath);
                element.filename = fi.completeBaseName();
                element.action = e.attribute("action");

                if (!element.path.isEmpty())
                    m_Elements.append(element);
            }
            n = n.nextSibling();
        }
    }
}

void SvnStatusModel::setPathGit(const QString &path, const QString &revision)
{
    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args;

    if (!revision.isEmpty())
    {
        // Показываем изменения для конкретного коммита
        args << "show" << "--name-status" << "--pretty=format:" << revision;
    }
    else
    {
        // Показываем изменения в рабочей директории (только модифицированные файлы)
        args << "status" << "--porcelain";
    }

    toolStartProcess(&proc, "git.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QString output = QString::fromUtf8(data);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    QDir main(m_Path);

    for (const QString &line : lines)
    {
        if (line.trimmed().isEmpty()) continue;

        SvnSatatusElement element;

        if (!revision.isEmpty())
        {
            // Формат git show: "M\tfile.txt"
            QRegularExpression regex("^(\\w+)\\t(.+)$");
            QRegularExpressionMatch match = regex.match(line);

            if (match.hasMatch())
            {
                QString status = match.captured(1);
                QString filePath = match.captured(2);

                element.action = gitStatusToAction(status);
                element.path = QDir::toNativeSeparators(filePath);
                element.fullpath = QDir::toNativeSeparators(main.absoluteFilePath(filePath));

                QFileInfo fi(element.fullpath);
                element.filename = fi.completeBaseName();

                m_Elements.append(element);
            }
        }
        else
        {
            // Формат git status --porcelain: " M file.txt"
            if (line.length() >= 4)
            {
                QString status = line.mid(0, 2).trimmed();
                QString filePath = line.mid(3).trimmed();

                // Показываем только измененные файлы (не новые, не удаленные)
                if (status == "M" || status == "MM") // Modified
                {
                    element.action = "modified";
                    element.path = QDir::toNativeSeparators(filePath);
                    element.fullpath = QDir::toNativeSeparators(main.absoluteFilePath(filePath));

                    QFileInfo fi(element.fullpath);
                    element.filename = fi.completeBaseName();

                    m_Elements.append(element);
                }
            }
        }
    }
}

QString SvnStatusModel::gitStatusToAction(const QString &gitStatus)
{
    static QHash<QString, QString> statusMap = {
        {"M", "modified"},
        {"A", "added"},
        {"D", "deleted"},
        {"R", "renamed"},
        {"C", "copied"},
        {"U", "updated"},
        {"?", "untracked"},
        {"!", "ignored"}
    };

    return statusMap.value(gitStatus, gitStatus);
}

VcsType SvnStatusModel::currentVcsType() const
{
    return m_VcsType;
}

QString SvnStatusModel::currentPath() const
{
    return m_Path;
}
