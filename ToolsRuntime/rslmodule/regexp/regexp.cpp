#include "regexp.h"
#include <QRegExp>
#include <QRegExpValidator>

RegExp::RegExp(const QString &_pattern)
    : QObject{}
{
    m_pRegExp.reset(new QRegExp(_pattern));
}

bool RegExp::isEmpty() const
{
    return m_pRegExp->isEmpty();
}

bool RegExp::isValid() const
{
    return m_pRegExp->isValid();
}

QString RegExp::pattern() const
{
    return m_pRegExp->pattern();
}

void RegExp::setPattern(const QString &pattern)
{
    m_pRegExp->setPattern(pattern);
}

bool RegExp::isMinimal() const
{
    return m_pRegExp->isMinimal();
}

void RegExp::setMinimal(bool minimal)
{
    m_pRegExp->setMinimal(minimal);
}

bool RegExp::exactMatch(const QString &str) const
{
    return m_pRegExp->exactMatch(str);
}

int RegExp::indexIn(const QString &str, int offset) const
{
    return m_pRegExp->indexIn(str, offset);
}

int RegExp::lastIndexIn(const QString &str, int offset) const
{
    return m_pRegExp->lastIndexIn(str, offset);
}

int RegExp::matchedLength() const
{
    return m_pRegExp->matchedLength();
}

int RegExp::captureCount() const
{
    return m_pRegExp->captureCount();
}

QStringList RegExp::capturedTexts() const
{
    return m_pRegExp->capturedTexts();
}

QStringList RegExp::capturedTexts()
{
    return m_pRegExp->capturedTexts();
}

QString RegExp::cap(int nth) const
{
    return m_pRegExp->cap(nth);
}

QString RegExp::cap(int nth)
{
    return m_pRegExp->cap(nth);
}

int RegExp::pos(int nth) const
{
    return m_pRegExp->pos(nth);
}

int RegExp::pos(int nth)
{
    return m_pRegExp->pos(nth);
}

QString RegExp::errorString() const
{
    return m_pRegExp->errorString();
}

QString RegExp::errorString()
{
    return m_pRegExp->errorString();
}

bool RegExpValidate(const QString &patern, const QString &str, int &pos)
{
    QRegExp rx(patern);
    QString value = str;

    QRegExpValidator validator(rx);
    return validator.validate(value, pos) == QValidator::Acceptable;
}
