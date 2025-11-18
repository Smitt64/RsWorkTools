#include "rttablecell.h"
#include "RTTextCursor.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntime.h"
#include <QTextTableCell>

RTTableCell::RTTableCell(const QTextTableCell &cell)
    : QObject{}
{
    m_QTextTableCell = cell;
}

RTTableCell::~RTTableCell()
{

}

RTTextCursor *RTTableCell::firstCursorPosition() const
{
    RTTextCursor *cursor = new RTTextCursor(m_QTextTableCell.firstCursorPosition());
    rslSetOwnerRsl(cursor);
    return cursor;
}

RTTextCursor *RTTableCell::lastCursorPosition() const
{
    RTTextCursor *cursor = new RTTextCursor(m_QTextTableCell.lastCursorPosition());
    rslSetOwnerRsl(cursor);
    return cursor;
}

int RTTableCell::column() const
{
    return m_QTextTableCell.column();
}

int RTTableCell::columnSpan() const
{
    return m_QTextTableCell.columnSpan();
}

bool RTTableCell::isValid() const
{
    return m_QTextTableCell.isValid();
}

int RTTableCell::row() const
{
    return m_QTextTableCell.row();
}

int RTTableCell::rowSpan() const
{
    return m_QTextTableCell.rowSpan();
}

QString RTTableCell::text() const
{
    QTextCursor cellCursor = m_QTextTableCell.firstCursorPosition();
    cellCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    QString cellText = cellCursor.selectedText();
    return toolReplaceUnicodeSymToOem(cellText);
}

void RTTableCell::setText(const QString &value)
{
    QTextCursor cellCursor = m_QTextTableCell.firstCursorPosition();
    cellCursor.select(QTextCursor::Document); // Select existing content
    cellCursor.removeSelectedText(); // Remove it
    cellCursor.insertText(value);
}
