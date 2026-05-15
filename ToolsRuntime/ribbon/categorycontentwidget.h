#ifndef CATEGORYCONTENTWIDGET_H
#define CATEGORYCONTENTWIDGET_H

#include "ToolsRuntime_global.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QScrollArea>
#include <QAbstractItemModel>
#include <QMap>
#include <QStackedWidget>
#include <QTreeWidget>

class QPushButton;
class QToolButton;
class QGroupBox;
class CategoryContentWidgetStyle;

TOOLSRUNTIME_EXPORT CategoryContentWidgetStyle *getCategoryContentStyle();

class TOOLSRUNTIME_EXPORT CategoryContentWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(LayoutType layoutType READ layoutType WRITE setLayoutType)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

public:
    enum LayoutType {
        LayoutListDetail,   // Список + детальная область
        LayoutGroups,       // Группы в сетке
        LayoutFullWidget,   // Один виджет на всю область
        LayoutLeftPanel     // Произвольный виджет слева + детальная область
    };
    Q_ENUM(LayoutType)

    explicit CategoryContentWidget(QWidget *parent = nullptr);
    virtual ~CategoryContentWidget();

    // Базовые методы
    LayoutType layoutType() const;
    void setLayoutType(LayoutType type);

    void updateListStyle();
    void updateOfficeStyle();  // Обновление Office-стиля
    QColor mixWithWhite(const QColor &base, int percent);
    QColor mixWithBlack(const QColor &base, int percent);

    // Для LayoutListDetail
    QListWidgetItem *addListItem(const QString &text, const QVariant &data = QVariant());
    QListWidgetItem *addListItem(const QString &text, const QIcon &icon, const QVariant &data = QVariant());
    QListWidgetItem *insertListItem(int row, const QString &text, const QIcon &icon, const QVariant &data = QVariant());
    QListWidgetItem *addListGroup(const QString &groupTitle);
    void clearList();
    void setDetailWidget(QWidget *detailWidget);
    void setListHeaderVisible(bool visible);
    void setListGroups(const QStringList &groups);
    QWidget* detailWidget() const;
    QListWidget* listWidget() const;

    QListWidgetItem *findListItem(const QString &text);
    void deleteListItem(const int &index);
    void deleteListItem(const QString &text);

    void setShowDetailHeader(bool show);
    bool showDetailHeader() const;
    void setDetailHeaderIconSize(const QSize &size);
    QSize detailHeaderIconSize() const;

    // Новые методы для добавления виджетов перед списком
    void addWidgetBeforeList(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void insertWidgetBeforeList(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void removeWidgetBeforeList(QWidget *widget);
    void clearWidgetsBeforeList();

    // Для LayoutGroups
    struct GroupInfo {
        QString title;
        QWidget *widget;
        int columnSpan;
    };

    void addGroup(const QString &groupTitle, QWidget *groupWidget, int columnSpan = 1);
    void insertGroup(int index, const QString &groupTitle, QWidget *groupWidget, int columnSpan = 1);
    void removeGroup(int index);
    void removeGroup(const QString &groupTitle);
    void clearGroups();
    void setGroupsColumns(int columns);
    void setGroupSpacing(int spacing);
    int groupCount() const;
    GroupInfo groupAt(int index) const;
    QString groupTitle(int index) const;
    QWidget* groupWidget(int index) const;

    // Для LayoutFullWidget
    void setMainWidget(QWidget *widget);
    QWidget* mainWidget() const;

    // Для LayoutLeftPanel
    void setLeftPanelWidget(QWidget *widget, int fixedWidth = 300);
    void setLeftPanelWidth(int width);
    QWidget* leftPanelWidget() const;
    void setLeftPanelVisible(bool visible);
    bool isLeftPanelVisible() const;

    // Состояние
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);

    // Скроллинг
    void enableScrolling(bool enable);
    void scrollToTop();

    void updateStyle();

    //static CategoryContentWidgetStyle *m_customStyle;

signals:
    void listItemSelected(const QVariant &data);
    void groupActionTriggered(const QString &groupId, const QString &actionId);
    void dataChanged();
    void layoutTypeChanged(LayoutType type);
    void detailHeaderClicked();

protected:
    void setupUI();
    void updateLayout();
    void updateGroupsLayout();

    virtual void onListItemSelected(const QVariant &data);
    virtual void onGroupAdded(int index);
    virtual void onGroupRemoved(int index);
    virtual void onLayoutChanged();

    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onListClicked(QListWidgetItem *item);

private:
    QWidget* createGroupWidget(const GroupInfo &group);
    void clearLayout(QLayout *layout);
    void setupCursorBehavior();
    void applyStyleRecursively(QWidget *widget);
    void applyStyleToWidget(QWidget *widget);
    void applyButtonStyle(QPushButton *button);
    void applyButtonStyle(QToolButton *button);
    void applyGroupBoxStyle(QGroupBox *groupBox);
    void updateListContainerLayout(); // Новый метод для обновления layout левой панели
    void updateDetailHeader();
    void createDetailHeader();

    LayoutType m_layoutType;
    bool m_readOnly;
    bool m_scrollingEnabled;

    // Общие компоненты
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Для LayoutListDetail
    QWidget *m_listContainer;
    QVBoxLayout *m_listContainerLayout; // Новый layout для левой панели
    QList<QWidget*> m_widgetsBeforeList; // Список виджетов перед списком
    QListWidget *m_listView;
    QWidget *m_detailContainer;
    QWidget *m_detailWidget;
    QStringList m_listGroups;
    QLabel *m_listHeaderLabel;

    // Для LayoutGroups
    QList<GroupInfo> m_groups;
    QWidget *m_groupsContainer;
    QGridLayout *m_groupsLayout;
    int m_groupsColumns;
    int m_groupSpacing;

    // Для LayoutFullWidget
    QWidget *m_mainWidget;

    // Для LayoutLeftPanel
    QWidget *m_leftPanelWidget;
    QWidget *m_leftPanelContainer;
    int m_leftPanelWidth;
    bool m_leftPanelVisible;

    bool m_showDetailHeader;
    QSize m_detailHeaderIconSize;
    QWidget *m_detailHeaderWidget;
    QLabel *m_detailHeaderIconLabel;
    QLabel *m_detailHeaderTitleLabel;
    QIcon m_currentDetailIcon;
    QString m_currentDetailTitle;
};

#endif // CATEGORYCONTENTWIDGET_H