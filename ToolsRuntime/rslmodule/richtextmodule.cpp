#include "toolsruntime.h"
#include "richtextmodule.h"
#include "rslexecutor.h"
#include "playrep.h"
#include "richtext/rttextcursor.h"
#include "richtext/rttablecell.h"
#include "richtext/rttable.h"
#include "richtext/rttextcharformat.h"
#include "richtext/rttextblockformat.h"
#include "richtext/rttextdocument.h"
#include "rsscript/registerobjlist.hpp"
#include <QTextDocument>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPlainTextEdit>

static void richTextGetDocument()
{
    enum
    {
        prm_Widget = 0,
    };

    QString title, msg, def;
    if (GetFuncParamType(prm_Widget) != QVariant::UserType)
        ThrowParamTypeError(prm_Widget);

    QTextDocument *pDocument = nullptr;
    QTextEdit *pEdit = GetFuncParam<QTextEdit*>(prm_Widget);
    if (pEdit)
        pDocument = pEdit->document();

    QTextBrowser *pBrowser = GetFuncParam<QTextBrowser*>(prm_Widget);
    if (pBrowser)
        pDocument = pBrowser->document();

    QPlainTextEdit *pPlain = GetFuncParam<QPlainTextEdit*>(prm_Widget);
    if (pPlain)
        pDocument = pPlain->document();

    if (pDocument)
    {
        RTTextDocument *pRtDoc = new RTTextDocument(pDocument);
        rslSetOwnerRsl(pRtDoc);
        SetReturnVal(pRtDoc);
    }
}

RichTextStaticModule::RichTextStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<RTTextCursor>();
    RegisterObjList::inst()->RegisterRslObject<RTTableCell>();
    RegisterObjList::inst()->RegisterRslObject<RTTable>();
    RegisterObjList::inst()->RegisterRslObject<RTTextCharFormat>();
    RegisterObjList::inst()->RegisterRslObject<RTTextDocument>();
    RegisterObjList::inst()->RegisterRslObject<RTTextBlockFormat>(GenInfoUseParentProps | GenInfoUseParentMeths);
    /*RegisterObjList::inst()->RegisterRslObject<IoFile>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoBuffer>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoTextStream>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<TemporaryFile>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<Process>(GenInfoUseParentProps | GenInfoUseParentMeths);

    RegisterObjList::inst()->RegisterRslObject<ProcessEnvironment>();
    RegisterObjList::inst()->RegisterRslObject<TemporaryDir>();

    RegisterObjList::inst()->RegisterRslObject<IoFileInfo>();
    RegisterObjList::inst()->RegisterRslObject<IoDir>();
    RegisterObjList::inst()->RegisterRslObject<IoNamespace>();*/
}

void RichTextStaticModule::Init()
{

}

void RichTextStaticModule::Proc()
{
    // Добавление констант для MoveOperation
    addConstant("CursorNoMove", QTextCursor::NoMove);
    addConstant("CursorStart", QTextCursor::Start);
    addConstant("CursorUp", QTextCursor::Up);
    addConstant("CursorStartOfLine", QTextCursor::StartOfLine);
    addConstant("CursorStartOfBlock", QTextCursor::StartOfBlock);
    addConstant("CursorStartOfWord", QTextCursor::StartOfWord);
    addConstant("CursorPreviousBlock", QTextCursor::PreviousBlock);
    addConstant("CursorPreviousCharacter", QTextCursor::PreviousCharacter);
    addConstant("CursorPreviousWord", QTextCursor::PreviousWord);
    addConstant("CursorLeft", QTextCursor::Left);
    addConstant("CursorWordLeft", QTextCursor::WordLeft);

    addConstant("CursorEnd", QTextCursor::End);
    addConstant("CursorDown", QTextCursor::Down);
    addConstant("CursorEndOfLine", QTextCursor::EndOfLine);
    addConstant("CursorEndOfWord", QTextCursor::EndOfWord);
    addConstant("CursorEndOfBlock", QTextCursor::EndOfBlock);
    addConstant("CursorNextBlock", QTextCursor::NextBlock);
    addConstant("CursorNextCharacter", QTextCursor::NextCharacter);
    addConstant("CursorNextWord", QTextCursor::NextWord);
    addConstant("CursorRight", QTextCursor::Right);
    addConstant("CursorWordRight", QTextCursor::WordRight);

    addConstant("CursorNextCell", QTextCursor::NextCell);
    addConstant("CursorPreviousCell", QTextCursor::PreviousCell);
    addConstant("CursorNextRow", QTextCursor::NextRow);
    addConstant("CursorPreviousRow", QTextCursor::PreviousRow);

    // Константы для MoveMode
    addConstant("MoveAnchor", QTextCursor::MoveAnchor);
    addConstant("KeepAnchor", QTextCursor::KeepAnchor);

    // Константы для SelectionType
    addConstant("SelectWord", QTextCursor::WordUnderCursor);
    addConstant("SelectLine", QTextCursor::LineUnderCursor);
    addConstant("SelectBlock", QTextCursor::BlockUnderCursor);
    addConstant("SelectDocument", QTextCursor::Document);

    // QFont::Capitalization
    addConstant("FontMixedCase", QFont::MixedCase);
    addConstant("FontAllUppercase", QFont::AllUppercase);
    addConstant("FontAllLowercase", QFont::AllLowercase);
    addConstant("FontSmallCaps", QFont::SmallCaps);
    addConstant("FontCapitalize", QFont::Capitalize);

    // QFont::HintingPreference
    addConstant("FontPreferDefaultHinting", QFont::PreferDefaultHinting);
    addConstant("FontPreferNoHinting", QFont::PreferNoHinting);
    addConstant("FontPreferVerticalHinting", QFont::PreferVerticalHinting);
    addConstant("FontPreferFullHinting", QFont::PreferFullHinting);

    // QFont::SpacingType
    addConstant("FontPercentageSpacing", QFont::PercentageSpacing);
    addConstant("FontAbsoluteSpacing", QFont::AbsoluteSpacing);

    // QFont::StyleHint
    addConstant("FontStyleHintAny", QFont::AnyStyle);
    addConstant("FontStyleHintSansSerif", QFont::SansSerif);
    addConstant("FontStyleHintSerif", QFont::Serif);
    addConstant("FontStyleHintTypeWriter", QFont::TypeWriter);
    addConstant("FontStyleHintDecorative", QFont::Decorative);
    addConstant("FontStyleHintMonospace", QFont::Monospace);
    addConstant("FontStyleHintFantasy", QFont::Fantasy);
    addConstant("FontStyleHintCursive", QFont::Cursive);
    addConstant("FontStyleHintSystem", QFont::System);

    // QFont::StyleStrategy
    addConstant("FontPreferDefault", QFont::PreferDefault);
    addConstant("FontPreferBitmap", QFont::PreferBitmap);
    addConstant("FontPreferDevice", QFont::PreferDevice);
    addConstant("FontPreferOutline", QFont::PreferOutline);
    addConstant("FontForceOutline", QFont::ForceOutline);
    addConstant("FontPreferMatch", QFont::PreferMatch);
    addConstant("FontPreferQuality", QFont::PreferQuality);
    addConstant("FontPreferAntialias", QFont::PreferAntialias);
    addConstant("FontNoAntialias", QFont::NoAntialias);
    addConstant("FontOpenGLCompatible", QFont::OpenGLCompatible);
    addConstant("FontForceIntegerMetrics", QFont::ForceIntegerMetrics);
    addConstant("FontNoSubpixelAntialias", QFont::NoSubpixelAntialias);
    addConstant("FontNoFontMerging", QFont::NoFontMerging);

    // QTextCharFormat::UnderlineStyle
    addConstant("UnderlineStyleNone", QTextCharFormat::NoUnderline);
    addConstant("UnderlineStyleSingle", QTextCharFormat::SingleUnderline);
    addConstant("UnderlineStyleDash", QTextCharFormat::DashUnderline);
    addConstant("UnderlineStyleDotLine", QTextCharFormat::DotLine);
    addConstant("UnderlineStyleDashDotLine", QTextCharFormat::DashDotLine);
    addConstant("UnderlineStyleDashDotDotLine", QTextCharFormat::DashDotDotLine);
    addConstant("UnderlineStyleWaveUnderline", QTextCharFormat::WaveUnderline);
    addConstant("UnderlineStyleSpellCheck", QTextCharFormat::SpellCheckUnderline);

    // QTextCharFormat::VerticalAlignment
    addConstant("VerticalAlignNormal", QTextCharFormat::AlignNormal);
    addConstant("VerticalAlignSuperScript", QTextCharFormat::AlignSuperScript);
    addConstant("VerticalAlignSubScript", QTextCharFormat::AlignSubScript);
    addConstant("VerticalAlignMiddle", QTextCharFormat::AlignMiddle);
    addConstant("VerticalAlignTop", QTextCharFormat::AlignTop);
    addConstant("VerticalAlignBottom", QTextCharFormat::AlignBottom);
    addConstant("VerticalAlignBaseline", QTextCharFormat::AlignBaseline);

    // Qt::Alignment
    addConstant("AlignLeft", Qt::AlignLeft);
    addConstant("AlignRight", Qt::AlignRight);
    addConstant("AlignHCenter", Qt::AlignHCenter);
    addConstant("AlignJustify", Qt::AlignJustify);
    addConstant("AlignTop", Qt::AlignTop);
    addConstant("AlignBottom", Qt::AlignBottom);
    addConstant("AlignVCenter", Qt::AlignVCenter);
    addConstant("AlignCenter", Qt::AlignCenter);
    addConstant("AlignAbsolute", Qt::AlignAbsolute);
    addConstant("AlignLeading", Qt::AlignLeading);
    addConstant("AlignTrailing", Qt::AlignTrailing);
    addConstant("AlignBaseline", Qt::AlignBaseline);

    // QTextBlockFormat::MarkerType
    addConstant("MarkerNoMarker", (int)QTextBlockFormat::MarkerType::NoMarker);
    addConstant("MarkerUnchecked", (int)QTextBlockFormat::MarkerType::Unchecked);
    addConstant("MarkerChecked", (int)QTextBlockFormat::MarkerType::Checked);

    // QTextBlockFormat::LineHeightTypes
    addConstant("LineHeightSingle", QTextBlockFormat::SingleHeight);
    addConstant("LineHeightProportional", QTextBlockFormat::ProportionalHeight);
    addConstant("LineHeightFixed", QTextBlockFormat::FixedHeight);
    addConstant("LineHeightMinimum", QTextBlockFormat::MinimumHeight);
    addConstant("LineHeightLineDistance", QTextBlockFormat::LineDistanceHeight);

    // QTextFormat::PageBreakFlags
    addConstant("PageBreakAuto", QTextFormat::PageBreak_Auto);
    addConstant("PageBreakAlwaysBefore", QTextFormat::PageBreak_AlwaysBefore);
    addConstant("PageBreakAlwaysAfter", QTextFormat::PageBreak_AlwaysAfter);

    // QTextOption::TabType
    addConstant("TabLeftTab", QTextOption::LeftTab);
    addConstant("TabRightTab", QTextOption::RightTab);
    addConstant("TabCenterTab", QTextOption::CenterTab);
    addConstant("TabDelimiterTab", QTextOption::DelimiterTab);

    // QTextDocument::FindFlags
    addConstant("FindBackward", QTextDocument::FindBackward);
    addConstant("FindCaseSensitively", QTextDocument::FindCaseSensitively);
    addConstant("FindWholeWords", QTextDocument::FindWholeWords);

    // QTextDocument::MetaInformation
    addConstant("DocumentTitle", QTextDocument::DocumentTitle);
    addConstant("DocumentUrl", QTextDocument::DocumentUrl);

    // QTextDocument::MarkdownFeatures
    addConstant("MarkdownNoHTML", QTextDocument::MarkdownNoHTML);
    addConstant("MarkdownDialectCommonMark", QTextDocument::MarkdownDialectCommonMark);
    addConstant("MarkdownDialectGitHub", QTextDocument::MarkdownDialectGitHub);

    // QTextDocument::Stacks
    addConstant("UndoStack", QTextDocument::UndoStack);
    addConstant("RedoStack", QTextDocument::RedoStack);
    addConstant("UndoAndRedoStacks", QTextDocument::UndoAndRedoStacks);

    // Qt::CursorMoveStyle
    addConstant("LogicalMoveStyle", Qt::LogicalMoveStyle);
    addConstant("VisualMoveStyle", Qt::VisualMoveStyle);

    RegisterObjList::inst()->AddObject<RTTextDocument>(true);
    RegisterObjList::inst()->AddObject<RTTextCursor>(true);
    RegisterObjList::inst()->AddObject<RTTextCharFormat>(true);
    RegisterObjList::inst()->AddObject<RTTextBlockFormat>(true);
    RegisterObjList::inst()->AddObject<RTTableCell>(false);
    RegisterObjList::inst()->AddObject<RTTable>(false);

    AddStdProc(V_GENOBJ, "richTextGetDocument", richTextGetDocument, 0);
}
