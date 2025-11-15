#ifndef RTTABLE_H
#define RTTABLE_H

#include <QObject>

class QTextTable;
class RTTextCursor;
class RTTableCell;
class RTTable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int columns READ columns CONSTANT)
    Q_PROPERTY(int rows READ rows CONSTANT)
public:
    RTTable(QTextTable *table);
    virtual ~RTTable();

    Q_INVOKABLE void appendColumns(int count);
    Q_INVOKABLE void appendRows(int count);

    Q_INVOKABLE RTTableCell *cellAt(int row, int column) const;
    Q_INVOKABLE RTTableCell *cellAt(int position) const;
    Q_INVOKABLE RTTableCell *cellAt(RTTextCursor *cursor) const;

    int columns() const;
    int rows() const;

    Q_INVOKABLE void insertColumns(int index, int columns);
    Q_INVOKABLE void insertRows(int index, int rows);

    Q_INVOKABLE void mergeCells(int row, int column, int numRows, int numCols);
    Q_INVOKABLE void mergeCells(RTTextCursor *cursor);

    Q_INVOKABLE void removeColumns(int index, int columns);
    Q_INVOKABLE void removeRows(int index, int rows);
    Q_INVOKABLE void resize(int rows, int columns);

    Q_INVOKABLE void splitCell(int row, int column, int numRows, int numCols);

private:
    QTextTable *m_pTable;
};

#endif // RTTABLE_H
