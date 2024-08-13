#include "rsloptionspage.h"
#include "ui_rsloptionspage.h"
#include "widgets/stringlisteditor.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntime.h"
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QSettings>
#include <QDir>
#include <QApplication>
#include <QDebug>

#define MACRO_GROUP "MacroDir"

class RslOptionsPagePrivate
{
    Q_DECLARE_PUBLIC(RslOptionsPage)
public:
    RslOptionsPagePrivate(RslOptionsPage *parent)
    {
        Q_Q(RslOptionsPage);
        q_ptr = parent;

        m_RslSettings.reset(new QSettings(toolFullFileNameFromDir("rsl.ini"), QSettings::IniFormat));
        pStringListEditor = new StringListEditor(parent);
        pStringListEditor->layout()->setMargin(0);
        //pStringListEditor->setCategoryes({"test1", "test2"});

        pStringListEditor->setHandler(&m_Handler);
        pStringListEditor->addStatic(RegisterObjList::inst()->staticMacroDir());
    }

    StdFolderListHandler m_Handler;
    StringListEditor *pStringListEditor;
    RslOptionsPage *q_ptr;

    QScopedPointer<QSettings> m_RslSettings;
};

RslOptionsPage::RslOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::RslOptionsPage)
    , d_ptr(new RslOptionsPagePrivate(this))
{
    Q_D(RslOptionsPage);
    ui->setupUi(this);

    QVBoxLayout *vbox = (QVBoxLayout*)layout();
    vbox->addWidget(d->pStringListEditor);

    QAbstractButton *pFolderBtn = d->pStringListEditor->button(StringListEditor::ButtonAddFolder);
    pFolderBtn->setVisible(true);

    connect(pFolderBtn, &QAbstractButton::clicked, [=]()
    {
        QString text;
        if (d->m_Handler.click(text, this))
            d->pStringListEditor->append(text);
    });
}

RslOptionsPage::~RslOptionsPage()
{
    delete ui;
}

int RslOptionsPage::save()
{
    Q_D(RslOptionsPage);
    QSettings *ps = settings();

    QStringList lst = d->pStringListEditor->stringList();
    ps->beginWriteArray(MACRO_GROUP, lst.size());
    for (int i = 0; i < lst.size(); i++)
    {
        ps->setArrayIndex(i);
        ps->setValue("value", lst.at(i));
    }
    ps->endArray();

    bool debugbreak = ui->checkDebugbreak->isChecked();
    d->m_RslSettings->beginGroup(QApplication::applicationName());
    d->m_RslSettings->setValue("debugbreak", debugbreak);
    d->m_RslSettings->endGroup();
    d->m_RslSettings->sync();

    return 0;
}

void RslOptionsPage::restore()
{
    Q_D(RslOptionsPage);
    QSettings *ps = settings();

    int size = ps->beginReadArray(MACRO_GROUP);
    for (int i = 0; i < size; ++i)
    {
        ps->setArrayIndex(i);
        d->pStringListEditor->append(ps->value("value").toString());
    }
    ps->endArray();

    d->m_RslSettings->beginGroup(QApplication::applicationName());
    ui->checkDebugbreak->setChecked(d->m_RslSettings->value("debugbreak").toBool());
    d->m_RslSettings->endGroup();
}
