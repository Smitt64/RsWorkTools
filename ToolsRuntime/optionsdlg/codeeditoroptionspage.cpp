#include "codeeditoroptionspage.h"
#include "ui_codeeditoroptionspage.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/highlighterstyle.h"
#include "codeeditor/codehighlighter.h"
#include "codeeditor/highlighteditmodel.h"
#include "toolsruntime.h"
#include <QInputDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QSettings>

class CodeEditorOptionsPagePrivate
{
    Q_DECLARE_PUBLIC(CodeEditorOptionsPage)
public:
    CodeEditorOptionsPagePrivate(CodeEditorOptionsPage *parent)
    {
        q_ptr = parent;

        m_pModel = new HighlightEditModel(parent);
        pEditor = new CodeEditor(parent);
    }

    void setCollorButtonStyle(QPushButton *btn, const QColor &color)
    {
        Q_Q(CodeEditorOptionsPage);

        QColor c = color;
        QPalette palette = q->palette();

        if (!color.isValid())
            c = palette.color(QPalette::Window);

        QString sstyle = QString("QPushButton { background-color: %1; border: 1px solid black; border-radius: 5px; }")
            .arg(c.name());

        QColor color2;
        double luminance = (color.red() * 0.299 + color.green() * 0.587 + color.blue() * 0.114) / 3;

        if (luminance >= 0.5)
            color2 = c.darker();
        else
            color2 = c.lighter(200);

        sstyle += QString("QPushButton:pressed {");
        sstyle += QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(%1, %2, %3, 255), stop:1 rgba(%4, %5, %6, 255))")
                      .arg(c.red())
                      .arg(c.green())
                      .arg(c.blue())
                      .arg(color2.red())
                      .arg(color2.green())
                      .arg(color2.blue());

        sstyle += "}";

        btn->setStyleSheet(sstyle);
    }

    void setEditorColors()
    {
        Q_Q(CodeEditorOptionsPage);
        setCollorButtonStyle(q->ui->editorBackground, m_pModel->editorBackground());
        setCollorButtonStyle(q->ui->editorCurrentWord, m_pModel->editorCurrentWord());
        setCollorButtonStyle(q->ui->editorCurrentLine, m_pModel->editorCurrentLine());

        setCollorButtonStyle(q->ui->linenumberBackground, m_pModel->linenumbersBackground());
        setCollorButtonStyle(q->ui->linenumberForeground, m_pModel->linenumbersForeground());
    }

    void setViewOnly(bool value)
    {
        Q_Q(CodeEditorOptionsPage);
        q->ui->editorBackground->setEnabled(!value);
        q->ui->editorCurrentWord->setEnabled(!value);
        q->ui->editorCurrentLine->setEnabled(!value);

        q->ui->linenumberBackground->setEnabled(!value);
        q->ui->linenumberForeground->setEnabled(!value);

        q->ui->linenumberBackgroundClear->setEnabled(!value);
        q->ui->linenumberForegroundClear->setEnabled(!value);

        if (value)
            q->ui->stackedWidget->setCurrentIndex(0);
        else
            q->ui->stackedWidget->setCurrentIndex(1);
    }

    void formatClicked(const QModelIndex &index)
    {
        Q_Q(CodeEditorOptionsPage);
        QPalette palette = q->palette();
        const QTextCharFormat &format = m_pModel->format(index.row());

        q->ui->fontButton->setFont(format.font());

        if (format.property(QTextFormat::ForegroundBrush).isValid())
            setCollorButtonStyle(q->ui->formatColor, format.foreground().color());
        else
            setCollorButtonStyle(q->ui->formatColor, palette.color(QPalette::Window));

        if (format.property(QTextFormat::BackgroundBrush).isValid())
            setCollorButtonStyle(q->ui->formatBackground, format.background().color());
        else
            setCollorButtonStyle(q->ui->formatBackground, palette.color(QPalette::Window));
    }

    void clearBackground()
    {
        Q_Q(CodeEditorOptionsPage);
        QPalette palette = q->palette();
        QModelIndex index = q->ui->listView->currentIndex();

        if (!index.isValid())
            return;

        QTextCharFormat &format = m_pModel->format(index.row());

        format.setProperty(QTextFormat::BackgroundBrush, QVariant());
        setCollorButtonStyle(q->ui->formatBackground, palette.color(QPalette::Window));
    }

    void fontButtonClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QModelIndex index = q->ui->listView->currentIndex();

        if (!index.isValid())
            return;

        QTextCharFormat &format = m_pModel->format(index.row());
        QString name = m_pModel->data(index).toString();
        QSharedPointer<StyleItem> style = m_pModel->style();

        QFont init = format.font();
        if (format.underlineStyle())
            init.setUnderline(true);

        init.setStrikeOut(format.fontStrikeOut());
        init.setItalic(format.fontItalic());

        if (format.fontWeight() == QFont::Bold)
            init.setBold(true);

        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, init, q,
                                          q->tr("Шрифт для стиля \"%1\"").arg(name),
                                          QFontDialog::MonospacedFonts);

        if (!ok)
            return;

        format.setFontItalic(font.italic());

        if (font.bold())
            format.setFontWeight(QFont::Bold);
        else
            format.setFontWeight(QFont::Normal);

        format.setFontUnderline(font.underline());
        format.setFontStrikeOut(font.strikeOut());

        format.setFont(font);
        style->save();

        q->ui->fontButton->setFont(font);
    }

    void backgroundColorClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QModelIndex index = q->ui->listView->currentIndex();

        if (!index.isValid())
            return;

        QTextCharFormat &format = m_pModel->format(index.row());
        QSharedPointer<StyleItem> style = m_pModel->style();

        QColor color = QColorDialog::getColor(format.background().color(),
                                              q,
                                              q->tr("Цвет фона"));

        if (!color.isValid())
            return;

        format.setBackground(color);
        setCollorButtonStyle(q->ui->formatBackground, color);

        style->save();
    }

    void textColorClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QModelIndex index = q->ui->listView->currentIndex();

        if (!index.isValid())
            return;

        QTextCharFormat &format = m_pModel->format(index.row());
        QSharedPointer<StyleItem> style = m_pModel->style();

        QColor color = QColorDialog::getColor(format.background().color(),
                                              q,
                                              q->tr("Цвет текста"));

        if (!color.isValid())
            return;

        format.setForeground(color);
        setCollorButtonStyle(q->ui->formatColor, color);

        style->save();
    }

    void linenumberBackgroundClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QSharedPointer<StyleItem> style = m_pModel->style();

        QColor color = QColorDialog::getColor(style->linenumbersBackground(),
                                              q,
                                              q->tr("Фон номеров строк"));

        if (!color.isValid())
            return;

        style->setLinenumbersBackground(color);
        setCollorButtonStyle(q->ui->linenumberBackground, color);

        style->save();
    }

    void linenumberForegroundClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QSharedPointer<StyleItem> style = m_pModel->style();

        QColor color = QColorDialog::getColor(style->linenumbersForeground(),
                                              q,
                                              q->tr("Цвет номеров строк:"));

        if (!color.isValid())
            return;

        style->setLinenumbersForeground(color);
        setCollorButtonStyle(q->ui->linenumberForeground, color);

        style->save();
    }

    void clearLinenumberBackgroundClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QSharedPointer<StyleItem> style = m_pModel->style();

        style->setLinenumbersBackground(QColor());
        setCollorButtonStyle(q->ui->linenumberBackground, QColor());

        style->save();
    }

    void clearLinenumberForegroundClicked()
    {
        Q_Q(CodeEditorOptionsPage);
        QSharedPointer<StyleItem> style = m_pModel->style();

        style->setLinenumbersForeground(QColor());
        setCollorButtonStyle(q->ui->linenumberForeground, QColor());

        style->save();
    }

    void rehighlight()
    {
        Q_Q(CodeEditorOptionsPage);
        ToolApplyHighlighter(pEditor, HighlighterCpp, q->ui->comboBox->currentText());
        pEditor->rehighlight();
    }

    void currentTextChanged(const QString &style)
    {
        Q_Q(CodeEditorOptionsPage);
        pEditor->setStyle(style);
        m_pModel->setStyle(style);
        m_pModel->reset();
        setEditorColors();
        rehighlight();
        q->ui->listView->update();

        bool isStatic = HighlighterStyle::inst()->isStatic(style);
        setViewOnly(isStatic);
        q->ui->deleteButton->setEnabled(!isStatic);
    }

    HighlightEditModel *m_pModel;
    CodeEditor *pEditor;
    QString m_Group, m_Key;
    CodeEditorOptionsPage *q_ptr;
};

CodeEditorOptionsPage::CodeEditorOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::CodeEditorOptionsPage)
    , d_ptr(new CodeEditorOptionsPagePrivate(this))
{
    Q_D(CodeEditorOptionsPage);
    ui->setupUi(this);

    ui->listView->setModel(d->m_pModel);
    ui->listView->setItemDelegate(d->m_pModel->delegate());
    ui->comboBox->addItems(HighlighterStyle::inst()->themes());
    QVBoxLayout *pLayout = new QVBoxLayout();
    ui->groupBox->setLayout(pLayout);

    pLayout->addWidget(d->pEditor);
    ui->deleteButton->setEnabled(false);
    d->pEditor->setReadOnly(true);
    d->pEditor->setPlainText(toolReadTextFileContent(":/codeeditoroptions/example.cpp"));

    d->m_pModel->setStyle(ui->comboBox->currentText());
    d->setEditorColors();

    d->setViewOnly(HighlighterStyle::inst()->isStatic(ui->comboBox->currentText()));
    ToolApplyHighlighter(d->pEditor, HighlighterCpp, ui->comboBox->currentText());

    connect(ui->comboBox, &QComboBox::currentTextChanged, [=](const QString &style)
    {
        d->currentTextChanged(style);
    });

    connect(ui->copyButton, &QPushButton::clicked, [=]()
    {
        QString old = ui->comboBox->currentText() + " копия";
        QString name = QInputDialog::getText(this, tr("Имя стиля"),
                                             tr("Укажите имя нового стиля:"),
                                             QLineEdit::Normal,
                                             old);

        if (name.isEmpty())
            return;

        QDir appDir = QDir(qApp->applicationDirPath());
        if (!appDir.cd(SYNTAXHIGHLIGHTER_FOLDER))
        {
            appDir.mkdir(SYNTAXHIGHLIGHTER_FOLDER);
            appDir.cd(SYNTAXHIGHLIGHTER_FOLDER);
        }

        QString filename = appDir.absoluteFilePath(QString("%1.json")
                                                       .arg(name));
        d->m_pModel->style()->save(filename);
        d->m_pModel->reset();

        ui->comboBox->clear();
        HighlighterStyle::inst()->loadStyle(filename);
        ui->comboBox->addItems(HighlighterStyle::inst()->themes());
        ui->comboBox->setCurrentText(name);
    });

    connect(ui->editorBackground, &QPushButton::clicked, [=]()
    {
        QColor color = QColorDialog::getColor(d->m_pModel->editorBackground(),
                               this,
                               tr("Фон редактора"));

        if (!color.isValid())
            return;

        d->setCollorButtonStyle(ui->editorBackground, color);

        QSharedPointer<StyleItem> style = d->m_pModel->style();
        style->setEditorBackground(color);
        d->rehighlight();
        style->save();
    });

    connect(ui->editorCurrentLine, &QPushButton::clicked, [=]()
    {
        QColor color = QColorDialog::getColor(d->m_pModel->editorCurrentLine(),
                                              this,
                                              tr("Фон текущей строки"));

        if (!color.isValid())
            return;

        d->setCollorButtonStyle(ui->editorCurrentLine, color);

        QSharedPointer<StyleItem> style = d->m_pModel->style();
        style->setEditorCurrentLine(color);
        d->rehighlight();
        style->save();
    });

    connect(ui->editorCurrentWord, &QPushButton::clicked, [=]()
    {
        QColor color = QColorDialog::getColor(d->m_pModel->editorCurrentWord(),
                                              this,
                                              tr("Фон текущего слова"));

        if (!color.isValid())
            return;

        d->setCollorButtonStyle(ui->editorCurrentWord, color);

        QSharedPointer<StyleItem> style = d->m_pModel->style();
        style->setEditorCurrentWord(color);
        d->rehighlight();
        style->save();
    });

    connect(ui->listView, &QListView::clicked, [=](const QModelIndex &index)
    {
        d->formatClicked(index);
    });

    connect(ui->fontButton, &QPushButton::clicked, [=]()
    {
        d->fontButtonClicked();
        d->m_pModel->reset();
        d->rehighlight();
    });

    connect(ui->clearBackground, &QPushButton::clicked, [=]()
    {
        d->clearBackground();
        d->m_pModel->reset();
        d->rehighlight();
    });

    connect(ui->formatBackground, &QPushButton::clicked, [=]()
    {
        d->backgroundColorClicked();
        d->m_pModel->reset();
        d->rehighlight();
    });

    connect(ui->formatColor, &QPushButton::clicked, [=]()
    {
        d->textColorClicked();
        d->m_pModel->reset();
        d->rehighlight();
    });

    connect(ui->linenumberBackground, &QPushButton::clicked, [=]()
    {
        d->linenumberBackgroundClicked();
        d->rehighlight();
        d->pEditor->update();
    });

    connect(ui->linenumberForeground, &QPushButton::clicked, [=]()
    {
        d->linenumberForegroundClicked();
        d->rehighlight();
        d->pEditor->update();
    });

    connect(ui->linenumberBackgroundClear, &QPushButton::clicked, [=]()
    {
        d->clearLinenumberBackgroundClicked();
        d->rehighlight();
        d->pEditor->update();
    });

    connect(ui->linenumberForegroundClear, &QPushButton::clicked, [=]()
    {
        d->clearLinenumberForegroundClicked();
        d->rehighlight();
        d->pEditor->update();
    });

    connect(ui->deleteButton, &QPushButton::clicked, [=]()
    {
        QString theme = ui->comboBox->currentText();

        if (QMessageBox::question(this, tr("Удаление"),
                                  tr("Удалить стиль <b>%1</b>?").arg(theme))
            == QMessageBox::Yes)
        {
            ui->comboBox->setCurrentIndex(0);
            d->currentTextChanged(ui->comboBox->currentText());

            HighlighterStyle::inst()->deleteStyle(theme);
            ui->comboBox->clear();

            QStringList lst = HighlighterStyle::inst()->themes();
            ui->comboBox->addItems(lst);
        }
    });
}

void CodeEditorOptionsPage::setSettingsKey(const QString &group, const QString &key)
{
    Q_D(CodeEditorOptionsPage);
    d->m_Group = group;
    d->m_Key = key;
}

CodeEditorOptionsPage::~CodeEditorOptionsPage()
{
    delete ui;
}

int CodeEditorOptionsPage::save()
{
    Q_D(CodeEditorOptionsPage);
    QSettings *ini = settings();

    if (!d->m_Group.isEmpty())
        ini->beginGroup(d->m_Group);
    else
        ini->beginGroup("General");

    ini->setValue(d->m_Key, ui->comboBox->currentText());
    ini->endGroup();

    return 0;
}

void CodeEditorOptionsPage::restore()
{
    Q_D(CodeEditorOptionsPage);

    QSettings *ini = settings();

    if (!d->m_Group.isEmpty())
        ini->beginGroup(d->m_Group);
    else
        ini->beginGroup("General");

    ui->comboBox->setCurrentText(ini->value(d->m_Key, "Default").toString());
    d->currentTextChanged(ui->comboBox->currentText());
    ini->endGroup();
}
