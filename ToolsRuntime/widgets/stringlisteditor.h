#ifndef STRINGLISTEDITOR_H
#define STRINGLISTEDITOR_H

#include <QWidget>
#include <QIcon>
#include "ToolsRuntime_global.h"

namespace Ui {
class StringListEditor;
}

class StringListEditorHandler
{
public:
    virtual QIcon buttonIcon() const { return QIcon(); }
    virtual bool click(QString &text, QWidget *parent) = 0;
};

class TOOLSRUNTIME_EXPORT StdFolderListHandler : public StringListEditorHandler
{
public:
    StdFolderListHandler() = default;
    virtual ~StdFolderListHandler() = default;

    QIcon buttonIcon() const Q_DECL_OVERRIDE;
    bool click(QString &text, QWidget *parent) Q_DECL_OVERRIDE;
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
