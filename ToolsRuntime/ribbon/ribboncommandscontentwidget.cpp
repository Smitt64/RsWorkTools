// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ribboncommandscontentwidget.h"
#include "ribboncommandstorage.h"
#include "ribboncommandlistwidget.h"
#include "SARibbon.h"

#include <QSettings>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QAction>
#include <QMessageBox>
#include <QSet>
#include <QHideEvent>

namespace {
    const int MacroActionTag = SARibbonActionsManager::UserDefineActionTag + 1;
}

RibbonCommandsContentWidget::RibbonCommandsContentWidget(QWidget *parent)
    : CategoryContentWidget(parent)
    , m_storage(new RibbonCommandStorage(this))
    , m_commandListWidget(nullptr)
    , m_customizeWidget(nullptr)
    , m_actionsManager(nullptr)
    , m_ribbonBar(nullptr)
    , m_stackedWidget(nullptr)
{
    setupUI();

    connect(m_storage, &RibbonCommandStorage::commandListChanged,
            this, &RibbonCommandsContentWidget::onCommandListChanged);
}

RibbonCommandsContentWidget::~RibbonCommandsContentWidget()
{
}

void RibbonCommandsContentWidget::setupUI()
{
    setLayoutType(LayoutListDetail);

    addListItem(tr("Команды"), 0);
    addListItem(tr("Настройка ленты"), 1);

    m_stackedWidget = new QStackedWidget(this);

    m_commandListWidget = new RibbonCommandListWidget(m_stackedWidget);
    m_commandListWidget->setStorage(m_storage);

    connect(m_commandListWidget, &RibbonCommandListWidget::commandRemoveRequested,
            this, &RibbonCommandsContentWidget::onCommandRemoveRequested);

    m_stackedWidget->addWidget(m_commandListWidget);

    setDetailWidget(m_stackedWidget);
}

void RibbonCommandsContentWidget::setRibbonBar(SARibbonBar *ribbonBar)
{
    if (m_ribbonBar == ribbonBar)
        return;

    m_ribbonBar = ribbonBar;

    if (m_customizeWidget)
    {
        m_customizeWidget->deleteLater();
        m_customizeWidget = nullptr;
    }

    updateCustomizeWidget();
}

void RibbonCommandsContentWidget::setActionsManager(SARibbonActionsManager *manager)
{
    if (m_actionsManager == manager)
        return;

    // Удаляем макрокоманды из старого менеджера
    if (m_actionsManager)
    {
        for (auto it = m_macroActions.begin(); it != m_macroActions.end(); ++it)
        {
            m_actionsManager->unregisteAction(it.value());
            if (!isActionOnRibbon(it.value()))
                delete it.value();
        }
        m_macroActions.clear();
    }

    m_actionsManager = manager;

    if (m_actionsManager)
        m_actionsManager->setTagName(MacroActionTag, tr("Макрокоманды"));

    syncMacroActions();
    updateCustomizeWidget();
}

QList<QAction*> RibbonCommandsContentWidget::macroActions() const
{
    return m_macroActions.values();
}

void RibbonCommandsContentWidget::syncMacroActions()
{
    if (!m_actionsManager)
        return;

    QSet<QString> storageIds;
    for (int i = 0; i < m_storage->count(); ++i)
    {
        RibbonCommandItem *item = m_storage->command(i);
        if (!item || item->id.isEmpty())
            continue;

        storageIds.insert(item->id);

        QAction *act = m_macroActions.value(item->id, nullptr);
        if (!act)
        {
            // Попробуем найти существующее действие в менеджере (например, созданное runtime)
            act = m_actionsManager->action(item->id);
            // Дополнительная проверка: это должно быть именно макродействие,
            // а не случайно совпавшее по ключу приложение
            if (act && act->objectName() == item->id && !act->data().toString().isEmpty())
            {
                // Берём под управление существующее действие
                m_macroActions[item->id] = act;
                emit macroActionAdded(act);
            }
            else
            {
                // Создаём новое действие
                act = new QAction(QIcon(item->icon), item->title, m_actionsManager);
                act->setObjectName(item->id);
                m_actionsManager->registeAction(act, MacroActionTag, item->id);
                m_macroActions[item->id] = act;
                emit macroActionAdded(act);
            }
        }

        act->setText(item->title);
        act->setIcon(QIcon(item->icon));
        act->setData(item->macrofile);
    }

    // Удаляем действия, которых больше нет в хранилище
    auto it = m_macroActions.begin();
    while (it != m_macroActions.end())
    {
        if (!storageIds.contains(it.key()))
        {
            m_actionsManager->unregisteAction(it.value());
            // Удаляем только если кнопка не осталась на панели Ribbon
            if (!isActionOnRibbon(it.value()))
                delete it.value();
            it = m_macroActions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void RibbonCommandsContentWidget::updateCustomizeWidget()
{
    if (!m_ribbonBar || !m_actionsManager)
        return;

    if (!m_customizeWidget)
    {
        m_customizeWidget = new SARibbonCustomizeWidget(m_ribbonBar, m_stackedWidget);
        m_stackedWidget->addWidget(m_customizeWidget);
    }

    m_customizeWidget->setupActionsManager(m_actionsManager);
    m_customizeWidget->updateModel();
}

void RibbonCommandsContentWidget::onCommandListChanged()
{
    syncMacroActions();
    updateCustomizeWidget();
}

bool RibbonCommandsContentWidget::isActionOnRibbon(QAction *action) const
{
    if (!m_ribbonBar || !action)
        return false;

    for (SARibbonCategory *category : m_ribbonBar->categoryPages())
    {
        for (SARibbonPannel *panel : category->pannelList())
        {
            for (SARibbonToolButton *btn : panel->ribbonToolButtons())
            {
                if (btn && btn->defaultAction() == action)
                    return true;
            }
        }
    }
    return false;
}

bool RibbonCommandsContentWidget::removeActionFromRibbon(QAction *action)
{
    if (!m_ribbonBar || !action)
        return false;

    bool removed = false;
    for (SARibbonCategory *category : m_ribbonBar->categoryPages())
    {
        for (SARibbonPannel *panel : category->pannelList())
        {
            if (panel->actions().contains(action))
            {
                panel->removeAction(action);
                removed = true;
            }
        }
    }

    if (removed)
    {
        m_ribbonBar->updateGeometry();
        m_ribbonBar->update();
    }

    return removed;
}

void RibbonCommandsContentWidget::onCommandRemoveRequested(int index)
{
    RibbonCommandItem *item = m_storage->command(index);
    if (!item)
        return;

    if (QMessageBox::question(this, tr("Удалить команду"),
                              tr("Вы действительно хотите удалить команду <b>\"%1\"</b>?")
                                  .arg(item->title)) != QMessageBox::Yes)
    {
        return;
    }

    QAction *act = m_macroActions.value(item->id, nullptr);
    if (!act && m_actionsManager)
        act = m_actionsManager->action(item->id);

    if (act)
        removeActionFromRibbon(act);

    m_storage->removeCommand(index);
}

void RibbonCommandsContentWidget::onListItemSelected(const QVariant &data)
{
    int page = data.toInt();
    if (!m_stackedWidget || page < 0 || page >= m_stackedWidget->count())
        return;

    m_stackedWidget->setCurrentIndex(page);
}

void RibbonCommandsContentWidget::hideEvent(QHideEvent *event)
{
    if (m_customizeWidget && m_customizeWidget->isCached())
    {
        m_customizeWidget->applys();
    }

    CategoryContentWidget::hideEvent(event);
}

void RibbonCommandsContentWidget::save(QSettings *settings)
{
    if (!settings)
        return;

    settings->beginGroup("RibbonCommands");

    QJsonArray commandsJson = m_storage->save();
    QJsonDocument doc(commandsJson);
    settings->setValue("CommandList", QString(doc.toJson(QJsonDocument::Compact).toBase64()));

    if (m_customizeWidget)
    {
        QByteArray xmlData;
        QXmlStreamWriter xml(&xmlData);
        xml.setAutoFormatting(true);
        xml.setAutoFormattingIndent(2);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        xml.setCodec("utf-8");
#endif
        xml.writeStartDocument();
        m_customizeWidget->toXml(&xml);
        xml.writeEndDocument();
        settings->setValue("RibbonCustomization", QString(xmlData.toBase64()));
    }

    settings->endGroup();
}

void RibbonCommandsContentWidget::load(QSettings *settings)
{
    if (!settings)
        return;

    settings->beginGroup("RibbonCommands");

    QByteArray commandsData = QByteArray::fromBase64(settings->value("CommandList").toByteArray());
    QJsonArray commandsJson = QJsonDocument::fromJson(commandsData).array();
    m_storage->load(commandsJson);

    QByteArray ribbonXml = QByteArray::fromBase64(settings->value("RibbonCustomization").toByteArray());
    if (!ribbonXml.isEmpty())
    {
        if (m_customizeWidget)
        {
            QXmlStreamReader xml1(ribbonXml);
            m_customizeWidget->fromXml(&xml1);
        }
        if (m_ribbonBar && m_actionsManager)
        {
            QXmlStreamReader xml2(ribbonXml);
            SARibbonCustomizeWidget::fromXml(&xml2, m_ribbonBar, m_actionsManager);
        }
        if (m_customizeWidget)
            m_customizeWidget->updateModel();
    }

    settings->endGroup();
}

