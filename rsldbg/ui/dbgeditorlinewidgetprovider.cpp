#include "dbgeditorlinewidgetprovider.h"
#include "qdebug.h"
#include <QPainter>
#include <QIcon>

DbgEditorLineWidgetProvider::DbgEditorLineWidgetProvider()
{

}

DbgEditorLineWidgetProvider::~DbgEditorLineWidgetProvider()
{

}

int DbgEditorLineWidgetProvider::width() const
{
    return 16;
}

void DbgEditorLineWidgetProvider::paint(QPainter *painter, const int &line, const QRect &rect)
{
    if (!m_IconInfo.contains(line))
        return;

    quint32 flags = m_IconInfo[line];

    if (flags & IconBreakpoint)
    {
        QIcon breakpoint = QIcon::fromTheme("Breakpoint");
        QRect rc(rect.topLeft(), QSize(rect.height(), rect.height()));
        painter->drawPixmap(rc, breakpoint.pixmap(rect.height()));
    }
}

void DbgEditorLineWidgetProvider::lineClick(const int &line, const Qt::MouseButton &button)
{
    if (button == Qt::LeftButton)
    {
        if (!(m_IconInfo[line] & IconBreakpoint))
            addItemId(IconBreakpoint, line);
        else
            removeItemId(IconBreakpoint, line);
    }
}

void DbgEditorLineWidgetProvider::addItemId(const int &id, const int &line)
{
     m_IconInfo[line] = m_IconInfo[line] | id;
}

void DbgEditorLineWidgetProvider::removeItemId(const int &id, const int &line)
{
    if (m_IconInfo.contains(line))
        m_IconInfo[line] = m_IconInfo[line] & (~id);
}
