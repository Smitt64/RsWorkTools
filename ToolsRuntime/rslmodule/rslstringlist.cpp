#include "rslmodule/rslStringList.h"
#include "rslexecutor.h"

StringListEx::StringListEx() :
    IterableObjectBase()
{
}

StringListEx::StringListEx(StringListEx *other) :
    IterableObjectBase()
{
    m_Container = other->m_Container;
}

StringListEx::StringListEx(const QStringList &list) :
    IterableObjectBase()
{
    m_Container = list;
}

StringListEx::~StringListEx()
{

}

int StringListEx::GetSize() const
{
    return m_Container.size();
}

QVariant StringListEx::GetNext()
{
    if (!_next())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant StringListEx::GetPrevious()
{
    if (!_previous())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant StringListEx::GetRecord(int index) const
{
    if (index < 0 || index >=  m_Container.size())
        return QVariant();

    return m_Container.at(index);
}

bool StringListEx::contains(const QString &str, int cs) const
{
    return m_Container.contains(str, (Qt::CaseSensitivity)cs);
}

class StringListEx *StringListEx::filter(const QString &str, int cs) const
{
    QStringList lst = m_Container.filter(str, (Qt::CaseSensitivity)cs);

    StringListEx *list = new StringListEx(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

int StringListEx::indexOf(const QString &str, int from) const
{
    return m_Container.indexOf(str, from);
}

QString StringListEx::join(const QString &separator) const
{
    return m_Container.join(separator);
}

int StringListEx::lastIndexOf(const QString &str, int from) const
{
    return m_Container.lastIndexOf(str, from);
}

void StringListEx::removeDuplicates()
{
    m_Container.removeDuplicates();
}

class StringListEx *StringListEx::replaceInStrings(const QString &before, const QString &after, int cs)
{
    QStringList lst = m_Container.replaceInStrings(before, after, (Qt::CaseSensitivity)cs);

    StringListEx *list = new StringListEx(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

void StringListEx::sort(int cs)
{
    m_Container.sort((Qt::CaseSensitivity)cs);
}

// --------------------------------------------------------------------------------------------------------

void StringListEx::append(const QString &str)
{
    m_Container.append(str);
}

const QString &StringListEx::at(const int &index)
{
    return m_Container.at(index);
}

const QString &StringListEx::back()
{
    return m_Container.back();
}

void StringListEx::clear()
{
    return m_Container.clear();
}

/*bool StringListEx::contains(const QString &value) const
{
    return m_Container.contains(value);
}*/

int StringListEx::count(const QString &value) const
{
    return m_Container.count(value);
}

bool StringListEx::endsWith(const QString &value) const
{
    return m_Container.endsWith(value);
}

const QString &StringListEx::first()
{
    return m_Container.first();
}

const QString &StringListEx::front()
{
    return m_Container.front();
}

void StringListEx::insert(int i, const QString &value)
{
    m_Container.insert(i, value);
}

const QString &StringListEx::last()
{
    return m_Container.last();
}

class StringListEx *StringListEx::mid(int pos, int length)
{
    QStringList lst = m_Container.mid(pos, length);

    StringListEx *list = new StringListEx(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

void StringListEx::move(int from, int to)
{
    m_Container.move(from, to);
}

void StringListEx::pop_back()
{
    m_Container.pop_back();
}

void StringListEx::pop_front()
{
    m_Container.pop_front();
}

void StringListEx::prepend(const QString &str)
{
    m_Container.prepend(str);
}

void StringListEx::push_back(const QString &str)
{
    m_Container.push_back(str);
}

void StringListEx::push_front(const QString &str)
{
    m_Container.push_front(str);
}

int StringListEx::removeAll(const QString &str)
{
    return m_Container.removeAll(str);
}

void StringListEx::removeAt(int pos)
{
    return m_Container.removeAt(pos);
}

void StringListEx::removeFirst()
{
    m_Container.removeFirst();
}

void StringListEx::removeLast()
{
    m_Container.removeLast();
}

bool StringListEx::removeOne(const QString &str)
{
    return m_Container.removeOne(str);
}

void StringListEx::replace(int i, const QString &value)
{
    m_Container.replace(i, value);
}

void StringListEx::reserve(int alloc)
{
    m_Container.reserve(alloc);
}

bool StringListEx::startsWith(const QString &str)
{
    return m_Container.startsWith(str);
}
