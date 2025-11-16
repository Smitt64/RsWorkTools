#ifndef RTTABLECELL_H
#define RTTABLECELL_H

#include <QObject>
#include <QTextTableCell>
#include "ToolsRuntime_global.h"

class QTextTableCell;
class RTTextCursor;
class TOOLSRUNTIME_EXPORT RTTableCell : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int column READ column)
    Q_PROPERTY(int columnSpan READ columnSpan)
    Q_PROPERTY(int row READ row)
    Q_PROPERTY(int rowSpan READ rowSpan)
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    explicit RTTableCell(const QTextTableCell &cell);
    virtual ~RTTableCell();

    int column() const;
    int columnSpan() const;

    bool isValid() const;

    int row() const;
    int rowSpan() const;

    QString text() const;
    void setText(const QString &value);

    Q_INVOKABLE RTTextCursor *firstCursorPosition() const;
    Q_INVOKABLE RTTextCursor *lastCursorPosition() const;

private:
    QTextTableCell m_QTextTableCell;
};

#endif // RTTABLECELL_H
