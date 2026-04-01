#ifndef STRINGLISTEDITOR_H
#define STRINGLISTEDITOR_H

#include <QWidget>
#include <QIcon>
#include "ToolsRuntime_global.h"

namespace Ui {
class StringListEditor;
}

class QStyleOptionViewItem;
class StringListEditorHandler
{
public:
    virtual QIcon buttonIcon() const { return _buttonIcon; }
    virtual QIcon rowIcon(const QModelIndex &index) const { return QIcon(); }
    virtual bool click(QString &text, QWidget *parent) = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) { return _size; }

    QIcon _buttonIcon;
    QSize _size;
};

class TOOLSRUNTIME_EXPORT StdFolderListHandler : public StringListEditorHandler
{
public:
    StdFolderListHandler();
    virtual ~StdFolderListHandler() = default;
    bool click(QString &text, QWidget *parent) Q_DECL_OVERRIDE;
    virtual QIcon rowIcon(const QModelIndex &index) const Q_DECL_OVERRIDE { return _rowIcon; }

    QIcon _rowIcon;
};

class QComboBox;
class QAbstractButton;
class StringListEditorPrivate;
class TOOLSRUNTIME_EXPORT StringListEditor : public QWidget
{
    Q_OBJECT

public:
    enum StandartButtons
    {
        ButtonNew,
        ButtonAddFolder,
        ButtonRemove
    };
    explicit StringListEditor(QWidget *parent = nullptr);
    ~StringListEditor();

    void append(const QString &value);
    void append(const QStringList &lst);

    void addStatic(const QString &value);
    void addStatic(const qint16 &id, const QString &value);

    void addStatic(const QStringList &lst);
    void addStatic(const qint16 &id, const QStringList &lst);
    void addList(const QStringList &lst);

    void setHandler(StringListEditorHandler *handler);
    StringListEditorHandler *handler();

    QStringList stringList();
    QComboBox *categoryWidget();
    QAbstractButton *button(StandartButtons btn);

    void setCategoryes(const QStringList &lst);

private:
    Ui::StringListEditor *ui;

    QScopedPointer<StringListEditorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(StringListEditor);
};

#endif // STRINGLISTEDITOR_H
