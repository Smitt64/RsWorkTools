#ifndef SPELLSTRINGSDLG_H
#define SPELLSTRINGSDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class SpellStringsDlg;
}

class SpellChecker;
class QTextCharFormat;
class SpellStringsDlgPrivate;
class TOOLSRUNTIME_EXPORT SpellStringsDlg : public QDialog
{
    Q_OBJECT

public:
    SpellStringsDlg(SpellChecker *spell, QWidget *parent = nullptr);
    ~SpellStringsDlg();

    void appendString(const QString &str, void *userData = nullptr);
    void setHighlightFormat(const QTextCharFormat &format);

    int count() const;
    QString value(const int &index) const;
    void *userData(const int &index) const;

protected:
    virtual bool skipWord(const QString &word);

private:
    SpellStringsDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(SpellStringsDlg);

    Ui::SpellStringsDlg *ui;
};

#endif // SPELLSTRINGSDLG_H
