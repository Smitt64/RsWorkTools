#include "variantlist.h"
#include "rslexecutor.h"

VariantList::VariantList()
    : IterableObjectBase{}
{

}

VariantList::VariantList(VariantList *other) :
    IterableObjectBase()
{
    m_Container = other->m_Container;
}

VariantList::VariantList(const QVariantList &list) :
    IterableObjectBase()
{
    m_Container = list;
}

VariantList::~VariantList()
{

}

int VariantList::GetSize() const
{
    return m_Container.size();
}

QVariant VariantList::GetNext()
{
    if (!_next())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant VariantList::GetPrevious()
{
    if (!_previous())
        return QVariant();

    return GetRecord(m_currentIndex);
}

QVariant VariantList::GetRecord(int index) const
{
    if (index < 0 || index >=  m_Container.size())
        return QVariant();

    return m_Container.at(index);
}

// --------------------------------------------------------------------------------------------------------

void VariantList::append(const QVariant &str)
{
    m_Container.append(str);
}

const QVariant &VariantList::at(const int &index)
{
    return m_Container.at(index);
}

const QVariant &VariantList::back()
{
    return m_Container.back();
}

void VariantList::clear()
{
    return m_Container.clear();
}

bool VariantList::contains(const QVariant &value) const
{
    return m_Container.contains(value);
}

int VariantList::count(const QVariant &value) const
{
    return m_Container.count(value);
}

bool VariantList::endsWith(const QVariant &value) const
{
    return m_Container.endsWith(value);
}

const QVariant &VariantList::first()
{
    return m_Container.first();
}

const QVariant &VariantList::front()
{
    return m_Container.front();
}

void VariantList::insert(int i, const QVariant &value)
{
    m_Container.insert(i, value);
}

const QVariant &VariantList::last()
{
    return m_Container.last();
}

class VariantList *VariantList::mid(int pos, int length)
{
    QVariantList lst = m_Container.mid(pos, length);

    VariantList *list = new VariantList(lst);
    SetObjectOwnerProp(list, RegisterInfoBase::RslOwner);

    return list;
}

void VariantList::move(int from, int to)
{
    m_Container.move(from, to);
}

void VariantList::pop_back()
{
    m_Container.pop_back();
}

void VariantList::pop_front()
{
    m_Container.pop_front();
}

void VariantList::prepend(const QVariant &str)
{
    m_Container.prepend(str);
}

void VariantList::push_back(const QVariant &str)
{
    m_Container.push_back(str);
}

void VariantList::push_front(const QVariant &str)
{
    m_Container.push_front(str);
}

int VariantList::removeAll(const QVariant &str)
{
    return m_Container.removeAll(str);
}

void VariantList::removeAt(int pos)
{
    return m_Container.removeAt(pos);
}

void VariantList::removeFirst()
{
    m_Container.removeFirst();
}

void VariantList::removeLast()
{
    m_Container.removeLast();
}

bool VariantList::removeOne(const QVariant &str)
{
    return m_Container.removeOne(str);
}

void VariantList::replace(int i, const QVariant &value)
{
    m_Container.replace(i, value);
}

void VariantList::reserve(int alloc)
{
    m_Container.reserve(alloc);
}

bool VariantList::startsWith(const QVariant &str)
{
    return m_Container.startsWith(str);
}
