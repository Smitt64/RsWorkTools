#include "errorsmodel.h"
#include <QApplication>
#include <QStyle>

typedef struct
{
    qint16 type;
    QString text;
    QDateTime time;
}FmtErrorStruct;

class ErrorsModelPrivate
{
    Q_DECLARE_PUBLIC(ErrorsModel)
public:
    ErrorsModelPrivate(ErrorsModel *obj)
    {
        q_ptr = obj;

        m_errorsCount = 0;
        m_warningsCount = 0;
        m_infoCount = 0;
        m_Iterator = QList<FmtErrorStruct>::iterator();
    }

    ErrorsModel *q_ptr;

    quint32 m_errorsCount, m_warningsCount, m_infoCount;
    QList<FmtErrorStruct> m_errors;
    QList<FmtErrorStruct>::iterator m_Iterator;
};

ErrorsModel::ErrorsModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new ErrorsModelPrivate(this))
{

}

ErrorsModel::~ErrorsModel()
{
    delete d_ptr;
}

int ErrorsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ErrorsModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ErrorsModel);
    QVariant value;

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 1)
        {
            QString text = d->m_errors.at(index.row()).text;
            text = text.remove(QRegExp("<[^>]*>")).simplified();
            value = text;
        }
        else if (index.column() == 0)
            value = d->m_errors.at(index.row()).time.toString("hh:mm:ss.zzz dd.MM.yyyy");
    }
    if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            qint16 type = d->m_errors.at(index.row()).type;
            switch(type)
            {
            case TypeError:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
                //qDebug() << m_errors.at(index.row()).text;
                break;
            case TypeWarning:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
                break;
            case TypeInfo:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
                break;
            }
        }
    }
    return value;
}

bool ErrorsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex ErrorsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex ErrorsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int ErrorsModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ErrorsModel);
    Q_UNUSED(parent);
    return d->m_errors.size();
}

Qt::ItemFlags ErrorsModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ErrorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList headers = QStringList()
            << tr("Событие") << tr("Текст");
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return headers.at(section);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

bool ErrorsModel::hasErrors() const
{
    Q_D(const ErrorsModel);
    bool result = false;
    if (d->m_errorsCount)
        result = true;
    return result;
}

bool ErrorsModel::hasWarnings() const
{
    Q_D(const ErrorsModel);
    bool result = false;
    if (d->m_warningsCount)
        result = true;
    return result;
}

int ErrorsModel::errorsCount() const
{
    Q_D(const ErrorsModel);
    return d->m_errorsCount;
}

int ErrorsModel::warningsCount() const
{
    Q_D(const ErrorsModel);
    return d->m_warningsCount;
}

int ErrorsModel::infoCount() const
{
    Q_D(const ErrorsModel);
    return d->m_infoCount;
}

bool ErrorsModel::isEmpty() const
{
    Q_D(const ErrorsModel);
    return d->m_errors.isEmpty();
}

void ErrorsModel::addError(const QString &text)
{
    appendError(text);
}

void ErrorsModel::addMessage(const QString &text)
{
    appendMessage(text);
}

void ErrorsModel::appendError(const QString &text, const qint16 &type, const QDateTime &dateTime)
{
    Q_D(ErrorsModel);

    FmtErrorStruct err;
    err.text = text;
    err.type = type;

    beginInsertRows(QModelIndex(), d->m_errors.size(), d->m_errors.size());
    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    d->m_errors.append(err);
    endInsertRows();
    emit newError(text);

    if (type == TypeError)
    {
        d->m_errorsCount ++;
        emit errorsCountChanged(d->m_errorsCount);
    }
    else if (type == TypeWarning)
    {
        d->m_warningsCount ++;
        emit warningsCountChanged(d->m_warningsCount);
    }
    else
    {
        d->m_infoCount ++;
        emit infoCountChanged(d->m_infoCount);
    }
}

void ErrorsModel::appendMessage(const QString &text, const QDateTime &dateTime)
{
    Q_D(ErrorsModel);

    FmtErrorStruct err;
    err.text = text;
    err.type = TypeInfo;
    d->m_infoCount ++;

    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    beginInsertRows(QModelIndex(), d->m_errors.size(), d->m_errors.size());
    d->m_errors.append(err);
    endInsertRows();
    emit newMessage(text);
    emit infoCountChanged(d->m_infoCount);
}

qint16 ErrorsModel::type(const qint32 &row) const
{
    Q_D(const ErrorsModel);

    if (row == -1)
        return (*d->m_Iterator).type;

    return d->m_errors.at(row).type;
}

QString ErrorsModel::text() const
{
    Q_D(const ErrorsModel);

    return (*d->m_Iterator).text;
}

QString ErrorsModel::textAt(const int &i) const
{
    Q_D(const ErrorsModel);

    if (i < 0 || i >= d->m_errors.size())
        return QString();
    return d->m_errors.at(i).text;
}

void ErrorsModel::clear()
{
    Q_D(ErrorsModel);

    beginResetModel();
    d->m_errors.clear();
    d->m_errorsCount = 0;
    d->m_warningsCount = 0;
    d->m_infoCount = 0;
    endResetModel();
}

bool ErrorsModel::next()
{
    Q_D(ErrorsModel);

    if (d->m_Iterator == QList<FmtErrorStruct>::iterator())
    {
        return first();
    }
    ++ d->m_Iterator;

    return d->m_Iterator != d->m_errors.end();
}

bool ErrorsModel::first()
{
    Q_D(ErrorsModel);

    d->m_Iterator = d->m_errors.begin();
    return d->m_Iterator != d->m_errors.end();
}
