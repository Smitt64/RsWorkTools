#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>

class QRegExp;
class RegExp : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE RegExp(const QString &_pattern);

    Q_INVOKABLE bool isEmpty() const;
    Q_INVOKABLE bool isValid() const;
    Q_INVOKABLE QString pattern() const;
    Q_INVOKABLE void setPattern(const QString &pattern);

    Q_INVOKABLE bool isMinimal() const;
    Q_INVOKABLE void setMinimal(bool minimal);

    Q_INVOKABLE bool exactMatch(const QString &str) const;

    Q_INVOKABLE int indexIn(const QString &str, int offset = 0) const;
    Q_INVOKABLE int lastIndexIn(const QString &str, int offset = -1) const;
    Q_INVOKABLE int matchedLength() const;

    Q_INVOKABLE int captureCount() const;
    Q_INVOKABLE QStringList capturedTexts() const;
    QStringList capturedTexts();
    Q_INVOKABLE QString cap(int nth = 0) const;
    QString cap(int nth = 0);
    Q_INVOKABLE int pos(int nth = 0) const;
    int pos(int nth = 0);
    Q_INVOKABLE QString errorString() const;
    QString errorString();

private:
    QScopedPointer<QRegExp> m_pRegExp;
};

bool RegExpValidate(const QString &patern, const QString &str, int &pos);

#endif // REGEXP_H
