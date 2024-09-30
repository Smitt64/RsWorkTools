#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <QObject>
#include "ToolsRuntime_global.h"

class SpellCheckerPrivate;
class TOOLSRUNTIME_EXPORT SpellChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid CONSTANT)
    Q_PROPERTY(QString userDictionary READ userDictionary CONSTANT)
public:
    SpellChecker(const QString &dictionaryPath, 
        const QString &userDictionary, 
        QObject *parent = nullptr);
    virtual ~SpellChecker();

    const QString &userDictionary() const;
    bool isValid() const;

    Q_INVOKABLE bool spell(const QString &word);
    Q_INVOKABLE QStringList suggest(const QString &word);
    Q_INVOKABLE void addToUserWordlist(const QString &word);

private:
    SpellCheckerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(SpellChecker); 
};

TOOLSRUNTIME_EXPORT QString spellGetDictionaryPath();
TOOLSRUNTIME_EXPORT void spellGetCheckerForLanguage(const QString &language, SpellChecker **checker);

#endif // SPELLCHECKER_H
