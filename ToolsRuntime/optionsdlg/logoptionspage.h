#ifndef LOGOPTIONSPAGE_H
#define LOGOPTIONSPAGE_H

#include <QWidget>
#include "OptionsPage.h"

namespace Ui {
class LogOptionsPage;
}

class LogOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit LogOptionsPage(QWidget *parent = nullptr);
    ~LogOptionsPage();

    void setPrefix(const QString &value);

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private slots:
    void OpenLogFile();
    void OnLoggingChack(bool on);

private:
    QString CreateRules();
    void UpdateDailog();

    QString m_Prefix;
    Ui::LogOptionsPage *ui;
};

#endif // LOGOPTIONSPAGE_H
