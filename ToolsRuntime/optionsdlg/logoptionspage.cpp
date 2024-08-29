#include "logoptionspage.h"
#include "qurl.h"
#include "ui_logoptionspage.h"
#include "toolsruntime.h"
#include <QDesktopServices>

LogOptionsPage::LogOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::LogOptionsPage)
{
    ui->setupUi(this);

    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(OpenLogFile()));
}

LogOptionsPage::~LogOptionsPage()
{
    delete ui;
}

void LogOptionsPage::OpenLogFile()
{
    QDesktopServices::openUrl(QUrl(toolLogginFileName()));
}

void LogOptionsPage::UpdateDailog()
{
    QFileInfo fi(toolLogginFileName());
    ui->lineEdit->setToolTip(toolLogginFileName());
    ui->lineEdit->setText(fi.fileName());
}

void LogOptionsPage::setPrefix(const QString &value)
{
    m_Prefix = value;
}

QString LogOptionsPage::CreateRules()
{
    QString rules;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *category = ui->treeWidget->topLevelItem(i);
        for (int j = 0; j < category->childCount(); j++)
        {
            QTreeWidgetItem *subcategory = category->child(j);
            if (!rules.isEmpty())
                rules = ";";
            rules += QString("%1.%2=%3")
                         .arg(category->text(0), subcategory->text(0))
                         .arg(subcategory->checkState(0) == Qt::Checked ? "true" : "false");
        }
    }

    return rules;
}

void LogOptionsPage::OnLoggingChack(bool on)
{
    if (!on)
        toolDisableLogging();
    else
    {
        if (!toolInitLogging(m_Prefix, CreateRules()))
            ui->groupBox->setChecked(false);
    }
    UpdateDailog();
}

int LogOptionsPage::save()
{
    if (toolIsLoggingEnabled())
        toolSetLoggingRules(CreateRules());

    return 0;
}

void LogOptionsPage::restore()
{
    const QStringList subcategorynames = QStringList()
    << "debug"
    << "info"
    << "warning"
    << "critical";

    QStringList lst = toolLoggingCategoryList();

    foreach (const QString &str, lst) {
        QTreeWidgetItem *category = new QTreeWidgetItem();
        category->setText(0, str);
        category->setCheckState(0, Qt::PartiallyChecked);
        ui->treeWidget->addTopLevelItem(category);

        int uncheckCount = 0;
        const QLoggingCategory &lc = toolLoggingCategory(str);
        if (QString(lc.categoryName()) == str)
        {
            foreach (const QString &sub, subcategorynames) {
                QTreeWidgetItem *subcategory = new QTreeWidgetItem();
                subcategory->setText(0, sub);

                if (sub == "debug")
                    subcategory->setCheckState(0, lc.isDebugEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "info")
                    subcategory->setCheckState(0, lc.isInfoEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "warning")
                    subcategory->setCheckState(0, lc.isWarningEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "critical")
                    subcategory->setCheckState(0, lc.isCriticalEnabled() ? Qt::Checked : Qt::Unchecked);

                if (subcategory->checkState(0) == Qt::Unchecked)
                    uncheckCount ++;

                category->addChild(subcategory);
            }

            if (uncheckCount == category->childCount())
                category->setCheckState(0, Qt::Unchecked);
            else if (uncheckCount == 0)
                category->setCheckState(0, Qt::Checked);
        }
    }

    ui->groupBox->setChecked(toolIsLoggingEnabled());
    UpdateDailog();

    connect(ui->groupBox, SIGNAL(toggled(bool)), SLOT(OnLoggingChack(bool)));
}
