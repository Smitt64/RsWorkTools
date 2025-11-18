#include "rttable.h"
#include "rttablecell.h"
#include "rttextcursor.h"
#include "rsscript/registerobjlist.hpp"
#include <QTextTable>
#include <QTextDocument>
#include <QTextFrame>

RTTable::RTTable(QTextTable *table) :
    QObject(),
    m_pTable(table)
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

// ----------------------------------------------------------------------------------

void findTablesInFrame(QTextFrame* frame, RTTableList *tables)
{
    if (!frame)
        return;

    QTextFrame::iterator it;
    for (it = frame->begin(); it != frame->end(); ++it)
    {
        QTextFrame* childFrame = it.currentFrame();
        if (childFrame)
        {
            QTextTable* table = qobject_cast<QTextTable*>(childFrame);
            if (table)
                tables->append(new RTTable(table));

            findTablesInFrame(childFrame, tables);
        }
    }
}

static void getAllTables(QTextDocument* document, RTTableList *tables)
{
    if (document)
        findTablesInFrame(document->rootFrame(), tables);
}

void RTGetDocumentTables(QTextDocument *document, RTTableList **tables)
{
    (*tables) = new RTTableList();

    getAllTables(document, *tables);
}

QVariant RTGetDocumentTables(QTextDocument *document)
{
    if (!document)
        return QVariant();

    RTTableList *tables = new RTTableList();
    findTablesInFrame(document->rootFrame(), tables);

    return QVariant::fromValue<QObject*>(tables);
}
