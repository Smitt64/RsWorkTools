#include "svnstatusmodel.h"
#include "toolsruntime.h"
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>

SvnStatusModel::SvnStatusModel(QObject *parent) :
    QAbstractTableModel(parent)
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

const SvnStatusElement &SvnStatusModel::element(const int &row) const
{
    return m_Elements[row];
}

void SvnStatusModel::setPath(const QString &path, const QString &revision)
{
    SvnInfoMap info = toolSvnGetRepoInfo(path);
    m_Path = path;
    beginResetModel();
    m_Elements.clear();

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
        SvnStatusElement element;
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
                SvnStatusElement element;

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

    endResetModel();
}
