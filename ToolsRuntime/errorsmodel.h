#ifndef ERRORSMODEL_H
#define ERRORSMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QDateTime>
#include "ToolsRuntime_global.h"

class ErrorsModelPrivate;
class TOOLSRUNTIME_EXPORT ErrorsModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasErrors READ hasErrors)
    Q_PROPERTY(bool hasWarnings READ hasWarnings)
    Q_PROPERTY(bool isEmpty READ isEmpty)

    Q_PROPERTY(int errorsCount READ errorsCount)
    Q_PROPERTY(int warningsCount READ warningsCount)
    Q_PROPERTY(int infoCount READ infoCount)
public:
    enum ErrorType
    {
        TypeError = 0,
        TypeWarning,
        TypeInfo
    };
    Q_ENUMS(ErrorType)

    Q_INVOKABLE ErrorsModel(QObject *parent = nullptr);
    virtual ~ErrorsModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex &index) const;
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool hasErrors() const;
    bool hasWarnings() const;
    bool isEmpty() const;

    int errorsCount() const;
    int warningsCount() const;
    int infoCount() const;

    Q_INVOKABLE void addError(const QString &text);
    Q_INVOKABLE void addMessage(const QString &text);
    Q_INVOKABLE void appendError(const QString &text, const qint16 &type = TypeError, const QDateTime &dateTime = QDateTime());
    Q_INVOKABLE void appendMessage(const QString &text, const QDateTime &dateTime = QDateTime());

    Q_INVOKABLE qint16 type(const qint32 &row = -1) const;
    Q_INVOKABLE QString text() const;
    Q_INVOKABLE QString textAt(const int &i) const;
    Q_INVOKABLE void clear();

    Q_INVOKABLE bool next();
    Q_INVOKABLE bool first();

signals:
    void errorsCountChanged(quint32);
    void warningsCountChanged(quint32);
    void infoCountChanged(quint32);
    void newError(QString);
    void newMessage(QString);

private:
    ErrorsModelPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ErrorsModel);
};

#endif // ERRORSMODEL_H
