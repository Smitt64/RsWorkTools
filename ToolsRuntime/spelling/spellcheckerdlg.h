#ifndef SPELLCHECKERDLG_H
#define SPELLCHECKERDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class SpellCheckerDlg;
}

class SpellChecker;
class SpellCheckerDlgPrivate;
class TOOLSRUNTIME_EXPORT SpellCheckerDlg : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString replacement READ replacement CONSTANT)
public:
    enum SpellCheckAction
    {
        AbortCheck = -1,
        AddToDict,
        IgnoreOnce,
        IgnoreAll,
        ReplaceOnce,
        ReplaceAll,
    };

    Q_INVOKABLE SpellCheckerDlg(QObject *checker, QWidget *parent = nullptr);
    ~SpellCheckerDlg();

    Q_INVOKABLE int checkWord(const QString &word);
    QString replacement() const;

private:
    Ui::SpellCheckerDlg *ui;

    SpellCheckerDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(SpellCheckerDlg);
};

#endif // SPELLCHECKERDLG_H
