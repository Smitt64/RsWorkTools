#include "rttable.h"
#include "rttablecell.h"
#include "rttextcursor.h"
#include "rsscript/registerobjlist.hpp"
#include <QTextTable>

RTTable::RTTable(QTextTable *table) :
    QObject()
{

}

RTTable::~RTTable()
{

}

void RTTable::appendColumns(int count)
{
    m_pTable->appendColumns(count);
}

void RTTable::appendRows(int count)
{
    m_pTable->appendRows(count);
}

RTTableCell *RTTable::cellAt(int row, int column) const
{
    QTextTableCell cell = m_pTable->cellAt(row, column);

    RTTableCell *pCell = new RTTableCell(cell);
    rslSetOwnerRsl(pCell);
    return pCell;
}

RTTableCell *RTTable::cellAt(int position) const
{
    QTextTableCell cell = m_pTable->cellAt(position);

    RTTableCell *pCell = new RTTableCell(cell);
    rslSetOwnerRsl(pCell);
    return pCell;
}

RTTableCell *RTTable::cellAt(RTTextCursor *cursor) const
{
    QTextTableCell cell = m_pTable->cellAt(cursor->textCursor());

    RTTableCell *pCell = new RTTableCell(cell);
    rslSetOwnerRsl(pCell);
    return pCell;
}

int RTTable::columns() const
{
    return m_pTable->columns();
}

int RTTable::rows() const
{
    return m_pTable->rows();
}

void RTTable::insertColumns(int index, int columns)
{
    m_pTable->insertColumns(index, columns);
}

void RTTable::insertRows(int index, int rows)
{
    m_pTable->insertRows(index, rows);
}

void RTTable::mergeCells(int row, int column, int numRows, int numCols)
{
    m_pTable->mergeCells(row, column, numRows, numCols);
}

void RTTable::mergeCells(RTTextCursor *cursor)
{
    m_pTable->mergeCells(cursor->textCursor());
}

void RTTable::removeColumns(int index, int columns)
{
    m_pTable->removeColumns(index, columns);
}

void RTTable::removeRows(int index, int rows)
{
    m_pTable->removeRows(index, rows);
}

void RTTable::resize(int rows, int columns)
{
    m_pTable->resize(rows, columns);
}

void RTTable::splitCell(int row, int column, int numRows, int numCols)
{
    m_pTable->splitCell(row, column, numRows, numCols);
}
