#include "rslmodule/rslstringlist.h"
#include "rslexecutor.h"

StringList::StringList() :
    IterableObjectBase()
{
}

StringList::StringList(StringList *other) :
    IterableObjectBase()
{
    m_Container = other->m_Container;
}

StringList::StringList(const QStringList &list) :
    IterableObjectBase()
{
    m_Container = list;
}

StringList::~StringList()
{

}

int StringList::GetSize() const
{
    return m_Container.size();
}

QVariant StringList::GetNext()
{
    if (!_next())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant StringList::GetPrevious()
{
    if (!_previous())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant StringList::GetRecord(int index) const
{
    if (index < 0 || index >=  m_Container.size())
        return QVariant();

    return m_Container.at(index);
}

bool StringList::contains(const QString &str, int cs) const
{
    return m_Container.contains(str, (Qt::CaseSensitivity)cs);
}

class StringList *StringList::filter(const QString &str, int cs) const
{
    QStringList lst = m_Container.filter(str, (Qt::CaseSensitivity)cs);

    StringList *list = new StringList(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

int StringList::indexOf(const QString &str, int from) const
{
    return m_Container.indexOf(str, from);
}

QString StringList::join(const QString &separator) const
{
    return m_Container.join(separator);
}

int StringList::lastIndexOf(const QString &str, int from) const
{
    return m_Container.lastIndexOf(str, from);
}

void StringList::removeDuplicates()
{
    m_Container.removeDuplicates();
}

class StringList *StringList::replaceInStrings(const QString &before, const QString &after, int cs)
{
    QStringList lst = m_Container.replaceInStrings(before, after, (Qt::CaseSensitivity)cs);

    StringList *list = new StringList(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

void StringList::sort(int cs)
{
    m_Container.sort((Qt::CaseSensitivity)cs);
}

// --------------------------------------------------------------------------------------------------------

void StringList::append(const QString &str)
{
    m_Container.append(str);
}

const QString &StringList::at(const int &index)
{
    return m_Container.at(index);
}

const QString &StringList::back()
{
    return m_Container.back();
}

void StringList::clear()
{
    return m_Container.clear();
}

/*bool StringList::contains(const QString &value) const
{
    return m_Container.contains(value);
}*/

int StringList::count(const QString &value) const
{
    return m_Container.count(value);
}

bool StringList::endsWith(const QString &value) const
{
    return m_Container.endsWith(value);
}

const QString &StringList::first()
{
    return m_Container.first();
}

const QString &StringList::front()
{
    return m_Container.front();
}

void StringList::insert(int i, const QString &value)
{
    m_Container.insert(i, value);
}

const QString &StringList::last()
{
    return m_Container.last();
}

class StringList *StringList::mid(int pos, int length)
{
    QStringList lst = m_Container.mid(pos, length);

    StringList *list = new StringList(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

void StringList::move(int from, int to)
{
    m_Container.move(from, to);
}

void StringList::pop_back()
{
    m_Container.pop_back();
}

void StringList::pop_front()
{
    m_Container.pop_front();
}

void StringList::prepend(const QString &str)
{
    m_Container.prepend(str);
}

void StringList::push_back(const QString &str)
{
    m_Container.push_back(str);
}

void StringList::push_front(const QString &str)
{
    m_Container.push_front(str);
}

int StringList::removeAll(const QString &str)
{
    return m_Container.removeAll(str);
}

void StringList::removeAt(int pos)
{
    return m_Container.removeAt(pos);
}

void StringList::removeFirst()
{
    m_Container.removeFirst();
}

void StringList::removeLast()
{
    m_Container.removeLast();
}

bool StringList::removeOne(const QString &str)
{
    return m_Container.removeOne(str);
}

void StringList::replace(int i, const QString &value)
{
    m_Container.replace(i, value);
}

void StringList::reserve(int alloc)
{
    m_Container.reserve(alloc);
}

bool StringList::startsWith(const QString &str)
{
    return m_Container.startsWith(str);
}
