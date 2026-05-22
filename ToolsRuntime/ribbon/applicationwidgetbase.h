#ifndef APPLICATIONWIDGETBASE_H
#define APPLICATIONWIDGETBASE_H

#include "ToolsRuntime_global.h"
#include "SARibbon.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QColor>
#include <QMap>
#include <QSpacerItem>

class AppOptionsContentWidget;
class TOOLSRUNTIME_EXPORT ApplicationWidgetBase : public SARibbonApplicationWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor menuPanelColor READ menuPanelColor WRITE setMenuPanelColor)

public:
    explicit ApplicationWidgetBase(SARibbonMainWindow *parent = nullptr);
    virtual ~ApplicationWidgetBase();

    // Управление цветом панели меню
    QColor menuPanelColor() const;
    void setMenuPanelColor(const QColor &color);

    // Добавление вкладки
    void addTab(const QString &title, QWidget *content);
    void insertTab(int index, const QString &title, QWidget *content);
    void removeTab(int index);
    void clearTabs();

    // Управление текущей вкладкой
    int currentTabIndex() const;
    void setCurrentTabIndex(int index);
    void setCurrentTab(const QString &title);

    // Получение вкладок
    QString tabTitle(int index) const;
    QWidget* tabContent(int index) const;
    int tabCount() const;

    AppOptionsContentWidget *optionsWidget();

    virtual void exec();

signals:
    void currentTabChanged(int index);
    void backButtonClicked();

protected:
    void setupUI();
    void updateMenuPanelStyle();
    void updateTabListStyle();
    void updateCategoryTitle();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onTabItemClicked(QListWidgetItem *item);
    void onBackButtonClicked();

private:
    void setupBackButton();
    QPixmap createColoredPixmap(const QByteArray &svgData, const QColor &color);

    // UI компоненты
    QWidget *m_menuPanel;
    QListWidget *m_tabList;
    QPushButton *m_backButton;
    QWidget *m_contentArea;
    QLabel *m_categoryTitle;
    QStackedWidget *m_stackedWidget;
    QVBoxLayout *m_menuLayout;

    // Данные
    QMap<int, QString> m_tabTitles;
    QMap<int, QWidget*> m_tabContents;
    QColor m_menuPanelColor;

    int m_currentIndex;

    // Данные для кнопки
    QPixmap m_backButtonNormalPixmap;
    QPixmap m_backButtonHoverPixmap;

    AppOptionsContentWidget *m_pOptionsWidget;
};

#endif // APPLICATIONWIDGETBASE_H