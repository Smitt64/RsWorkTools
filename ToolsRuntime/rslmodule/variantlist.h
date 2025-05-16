#ifndef VARIANTLIST_H
#define VARIANTLIST_H

#include "iterableobjectbase.h"
#include "rslmodule/iterableobjectbase.h"

class TOOLSRUNTIME_EXPORT VariantList : public IterableObjectBase
{
    Q_OBJECT
public:
    Q_INVOKABLE VariantList();
    Q_INVOKABLE VariantList(VariantList *other);
    VariantList(const QVariantList &list);
    virtual ~VariantList();

    Q_INVOKABLE void append(const QVariant &str);
    Q_INVOKABLE const QVariant &at(const int &index);
    Q_INVOKABLE const QVariant &back();
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool contains(const QVariant &value) const;
    Q_INVOKABLE int count(const QVariant &value) const;
    Q_INVOKABLE bool endsWith(const QVariant &value) const;
    Q_INVOKABLE const QVariant &first();
    Q_INVOKABLE const QVariant &front();
    Q_INVOKABLE void insert(int i, const QVariant &value);
    Q_INVOKABLE const QVariant &last();
    Q_INVOKABLE VariantList *mid(int pos, int length = -1);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void pop_back();
    Q_INVOKABLE void pop_front();
    Q_INVOKABLE void prepend(const QVariant &str);
    Q_INVOKABLE void push_back(const QVariant &str);
    Q_INVOKABLE void push_front(const QVariant &str);
    Q_INVOKABLE int removeAll(const QVariant &str);
    Q_INVOKABLE void removeAt(int pos);
    Q_INVOKABLE void removeFirst();
    Q_INVOKABLE void removeLast();
    Q_INVOKABLE bool removeOne(const QVariant &str);
    Q_INVOKABLE void replace(int i, const QVariant &value);
    Q_INVOKABLE void reserve(int alloc);
    Q_INVOKABLE bool startsWith(const QVariant &str);

    Q_INVOKABLE void fromOther(const QVariantList &other);
    const QVariantList &container() const;

protected:
    virtual int GetSize() const Q_DECL_OVERRIDE;
    virtual QVariant GetNext() Q_DECL_OVERRIDE;
    virtual QVariant GetPrevious() Q_DECL_OVERRIDE;
    virtual QVariant GetRecord(int index) const Q_DECL_OVERRIDE;

private:
    QVariantList m_Container;
};

Q_DECLARE_OPAQUE_POINTER(VariantList);

#endif // VARIANTLIST_H
