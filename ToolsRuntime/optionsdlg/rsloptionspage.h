#ifndef RSLOPTIONSPAGE_H
#define RSLOPTIONSPAGE_H

#include <QWidget>
#include "OptionsPage.h"

namespace Ui {
class RslOptionsPage;
}

class RslOptionsPagePrivate;
class RslOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit RslOptionsPage(QWidget *parent = nullptr);
    ~RslOptionsPage();

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private:
    Ui::RslOptionsPage *ui;

    QScopedPointer<RslOptionsPagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(RslOptionsPage);
};

#endif // RSLOPTIONSPAGE_H
