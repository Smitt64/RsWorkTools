#ifndef RIBBONABOUTDLG_H
#define RIBBONABOUTDLG_H

#include <QDialog>
#include <QColor>
#include "ToolsRuntime_global.h"

class RibbonAboutDlgPrivate;

class TOOLSRUNTIME_EXPORT RibbonAboutDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RibbonAboutDlg(const QString &config,
                            const QColor &accentColor = QColor(0x21, 0x73, 0x46),
                            QWidget *parent = nullptr);
    ~RibbonAboutDlg();

    void setAccentColor(const QColor &color);

private slots:
    void urlChanged(const QUrl &url);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void applyStyles();

    RibbonAboutDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RibbonAboutDlg);
};

#endif // RIBBONABOUTDLG_H