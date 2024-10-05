#include "spellchecker.h"
#include <QGlobalStatic>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QRegularExpression>
#include <QDir>
#include <QApplication>
#include "hunspell.hxx"

static QMap<QString, SpellChecker*> sSpellCheckers;

class SpellCheckerPrivate
{
    Q_DECLARE_PUBLIC(SpellChecker)
public:
    SpellCheckerPrivate(SpellChecker *obj) :
        q_ptr(obj)
    {
    }

    void init(const QString &dictionaryPath, const QString &userDictionary)
    {
        QString dictFile = dictionaryPath + ".dic";
        QString affixFile = dictionaryPath + ".aff";

        QByteArray dictFilePathBA = dictFile.toLocal8Bit();
        QByteArray affixFilePathBA = affixFile.toLocal8Bit();

        m_UserDictionary = userDictionary;
        _encoding = QStringLiteral("ISO8859-15");

        QFile _affixFile(affixFile);
        if (_affixFile.open(QIODevice::ReadOnly)) 
        {
            pHunspell.reset(new Hunspell(affixFilePathBA.constData(),
                dictFilePathBA.constData()));

            QTextStream stream(&_affixFile);
            QRegularExpression enc_detector(
                QStringLiteral("^\\s*SET\\s+([A-Z0-9\\-]+)\\s*"),
                QRegularExpression::CaseInsensitiveOption);

            QString sLine;
            QRegularExpressionMatch match;

            while (!stream.atEnd())
            {
                sLine = stream.readLine();
                if (sLine.isEmpty()) 
                    continue;

                match = enc_detector.match(sLine);

                if (match.hasMatch()) 
                {
                    _encoding = match.captured(1);
                    break;
                }
            }

            _affixFile.close();
        }

        _codec = QTextCodec::codecForName(this->_encoding.toLatin1().constData());

        QFile userDictonaryFile(m_UserDictionary);
        if (!m_UserDictionary.isEmpty() && userDictonaryFile.exists())
        {
            if (userDictonaryFile.open(QIODevice::ReadOnly))
            {
                QTextStream stream(&userDictonaryFile);
                for (QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine())
                    put_word(word);

                userDictonaryFile.close();
            }
        }
    }

    void put_word(const QString &word)
    {
        if (!pHunspell)
            return;

        pHunspell->add(_codec->fromUnicode(word).constData());
    }

    QTextCodec *_codec;
    QString m_UserDictionary;
    SpellChecker *q_ptr;
    QString _encoding;
    QScopedPointer<Hunspell> pHunspell;
};

SpellChecker::SpellChecker(const QString &dictionaryPath, 
        const QString &userDictionary, 
        QObject *parent)
    : QObject{parent},
    d_ptr(new SpellCheckerPrivate(this))
{
    d_ptr->init(dictionaryPath, userDictionary);
}

SpellChecker::~SpellChecker()
{
    delete d_ptr;
}

const QString &SpellChecker::userDictionary() const
{
    Q_D(const SpellChecker);
    return d->m_UserDictionary;
}

bool SpellChecker::isValid() const
{
    Q_D(const SpellChecker);
    return !d->pHunspell.isNull();
}

bool SpellChecker::spell(const QString &word)
{
    Q_D(SpellChecker);
    return d->pHunspell->spell(d->_codec->fromUnicode(word).toStdString());
}

QStringList SpellChecker::suggest(const QString &word)
{
    Q_D(SpellChecker);
    QStringList suggestions;
    std::vector<std::string> wordlist;
    wordlist = d->pHunspell->suggest(d->_codec->fromUnicode(word).toStdString());

    int numSuggestions = static_cast<int>(wordlist.size());
    if (numSuggestions > 0)
    {
        suggestions.reserve(numSuggestions);
        for (int i = 0; i < numSuggestions; i++)
        {
            suggestions << d->_codec->toUnicode(
                       QByteArray::fromStdString(wordlist[i]));
        }
    }

    return suggestions;
}

void SpellChecker::addToUserWordlist(const QString &word)
{
    Q_D(SpellChecker);
    d->put_word(word);

    if (!d->m_UserDictionary.isEmpty())
    {
        QFile userDictonaryFile(d->m_UserDictionary);
        if (userDictonaryFile.open(QIODevice::Append))
        {
            QTextStream stream(&userDictonaryFile);
            stream << word << "\n";
            userDictonaryFile.close();
        }
    }
}

QString spellGetDictionaryPath()
{
    QDir dir(qApp->applicationDirPath());

    if (!dir.cd("dictionaries"))
        return QString();

    return dir.absolutePath();
}

void spellGetCheckerForLanguage(const QString &language, SpellChecker **checker)
{
    if (sSpellCheckers.contains(language))
        *checker = sSpellCheckers[language];
    else
    {
        QString dictionaryPath = spellGetDictionaryPath() + "/" + language;
        QString userDictionary = dictionaryPath + ".user";
        
        SpellChecker *pchecker = new SpellChecker(dictionaryPath, userDictionary);
        if (!pchecker->isValid())
        {
            delete pchecker;
            pchecker = nullptr;
        }
        else
            sSpellCheckers[language] = pchecker;

        *checker = pchecker;
    }
}
