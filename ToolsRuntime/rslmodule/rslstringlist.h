#ifndef StringListExt__H_
#define StringListExt__H_

#include "rslmodule/iterableobjectbase.h"

class TOOLSRUNTIME_EXPORT StringListEx : public IterableObjectBase
{
    Q_OBJECT
public:
    Q_INVOKABLE StringListEx();
    Q_INVOKABLE StringListEx(StringListEx *other);
    StringListEx(const QStringList &list);
    virtual ~StringListEx();

    Q_INVOKABLE void append(const QString &str);
    Q_INVOKABLE const QString &at(const int &index);
    Q_INVOKABLE const QString &back();
    Q_INVOKABLE void clear();
    //Q_INVOKABLE bool contains(const QString &value) const;
    Q_INVOKABLE int count(const QString &value) const;
    Q_INVOKABLE bool endsWith(const QString &value) const;
    Q_INVOKABLE const QString &first();
    Q_INVOKABLE const QString &front();
    Q_INVOKABLE void insert(int i, const QString &value);
    Q_INVOKABLE const QString &last();
    Q_INVOKABLE StringListEx *mid(int pos, int length = -1);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void pop_back();
    Q_INVOKABLE void pop_front();
    Q_INVOKABLE void prepend(const QString &str);
    Q_INVOKABLE void push_back(const QString &str);
    Q_INVOKABLE void push_front(const QString &str);
    Q_INVOKABLE int removeAll(const QString &str);
    Q_INVOKABLE void removeAt(int pos);
    Q_INVOKABLE void removeFirst();
    Q_INVOKABLE void removeLast();
    Q_INVOKABLE bool removeOne(const QString &str);
    Q_INVOKABLE void replace(int i, const QString &value);
    Q_INVOKABLE void reserve(int alloc);
    Q_INVOKABLE bool startsWith(const QString &str);

    Q_INVOKABLE bool contains(const QString &str, int cs = Qt::CaseSensitive) const;
    Q_INVOKABLE StringListEx *filter(const QString &str, int cs = Qt::CaseSensitive) const;
    Q_INVOKABLE int indexOf(const QString &str, int from = 0) const;
    Q_INVOKABLE QString join(const QString &separator) const;
    Q_INVOKABLE int lastIndexOf(const QString &str, int from = -1) const;
    Q_INVOKABLE void removeDuplicates();
    Q_INVOKABLE StringListEx *replaceInStrings(const QString &before, const QString &after, int cs = Qt::CaseSensitive);
    Q_INVOKABLE void sort(int cs = Qt::CaseSensitive);

    const QStringList &container() const;

protected:
    virtual int GetSize() const Q_DECL_OVERRIDE;
    virtual QVariant GetNext() Q_DECL_OVERRIDE;
    virtual QVariant GetPrevious() Q_DECL_OVERRIDE;
    virtual QVariant GetRecord(int index) const Q_DECL_OVERRIDE;

private:
    QStringList m_Container;
};

Q_DECLARE_OPAQUE_POINTER(StringListEx);

#endif // StringListExt__H_
