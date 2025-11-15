QT += gui widgets webengine webenginewidgets xml uitools sql testlib xmlpatterns

TEMPLATE = lib
DEFINES += TOOLSRUNTIME_LIBRARY Q_WS_WIN

CONFIG += c++17
RC_FILE = res/winfmt.rc

include($$PWD/SARibbon.pri)
include(../../configtools.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    7zip/archive.cpp \
    aboutdlg.cpp \
    codeeditor/codeeditor.cpp \
    codeeditor/codehighlighter.cpp \
    codeeditor/cppcodehighlighter.cpp \
    codeeditor/highlighteditmodel.cpp \
    codeeditor/highlighterstyle.cpp \
    codeeditor/rslcodehighlighter.cpp \
    codeeditor/sqlcodehighlighter.cpp \
    codeeditor/xmlcodehighlighter.cpp \
    errordlg.cpp \
    errorsmodel.cpp \
    iconlibdlg.cpp \
    levenshtein.cpp \
    netapi/apiuser.cpp \
    optionsdlg/OptionsPage.cpp \
    optionsdlg/codeeditoroptionspage.cpp \
    optionsdlg/commandsoptions.cpp \
    optionsdlg/commandsstorage.cpp \
    optionsdlg/jsonsettings.cpp \
    optionsdlg/logoptionspage.cpp \
    optionsdlg/optionsdlg.cpp \
    optionsdlg/rsloptionspage.cpp \
    optionsdlg/styleoptionspage.cpp \
    optionsdlg/updateoptionspage.cpp \
    propertywidgetmapper.cpp \
    rslexecutor.cpp \
    rslmodule/dialogs/message_box.cpp \
    rslmodule/io/iobuffer.cpp \
    rslmodule/io/iodevice.cpp \
    rslmodule/io/iodir.cpp \
    rslmodule/io/iofile.cpp \
    rslmodule/io/iofileinfo.cpp \
    rslmodule/io/ionamespace.cpp \
    rslmodule/io/iotextstream.cpp \
    rslmodule/io/process.cpp \
    rslmodule/io/temporaryfile.cpp \
    rslmodule/iostaticmodule.cpp \
    rslmodule/iterableobjectbase.cpp \
    rslmodule/regexp/regexp.cpp \
    rslmodule/regexpmodule.cpp \
    rslmodule/richtext/rttable.cpp \
    rslmodule/richtext/rttablecell.cpp \
    rslmodule/richtext/rttextblockformat.cpp \
    rslmodule/richtext/rttextcharformat.cpp \
    rslmodule/richtext/rttextcursor.cpp \
    rslmodule/richtext/rttextdocument.cpp \
    rslmodule/richtextmodule.cpp \
    rslmodule/rslstringlist.cpp \
    rslmodule/sql/sqldatabase.cpp \
    rslmodule/sql/sqlquery.cpp \
    rslmodule/sqlstaticmodule.cpp \
    rslmodule/stddialogs.cpp \
    rslmodule/toolsruntimemodule.cpp \
    rslmodule/toolsruntinersl.cpp \
    rslmodule/types/QByteArrayRsl.cpp \
    rslmodule/types/point.cpp \
    rslmodule/types/rect.cpp \
    rslmodule/types/size.cpp \
    rslmodule/uistaticmodule.cpp \
    rslmodule/rslstr/strlib.cpp \
    rslmodule/variantlist.cpp \
    rsscript/Methods.cpp \
    rsscript/Properties.cpp \
    rsscript/Signals.cpp \
    rsscript/registerinfobase.cpp \
    rsscript/registerobjlist.cpp \
    rsscript/rsllistitemcontainer.cpp \
    rsscript/rsllistwrapper.cpp \
    rsscript/rslobjconnections.cpp \
    rsscript/rslstaticmodule.cpp \
    rsscript/typeinfofunc.cpp \
    spelling/spellchecker.cpp \
    spelling/spellcheckerdlg.cpp \
    spelling/spellstringsdlg.cpp \
    svn/svnstatusmodel.cpp \
    toolsqlconverter.cpp \
    toolsruntime.cpp \
    updatechecker.cpp \
    updatecheckermessagebox.cpp \
    widgets/codeinputdialog.cpp \
    widgets/doubleprogressdialog.cpp \
    widgets/doubleprogressdialogprivate.cpp \
    widgets/selectactiondlg.cpp \
    widgets/stringlisteditor.cpp \
    widgets/treeheadervew/gridtableheadermodel.cpp \
    widgets/treeheadervew/gridtableheaderview.cpp \
    widgets/treeheadervew/tableheaderitem.cpp \
    windowactionsregistry.cpp \
    xmlvalidator.cpp

HEADERS += \
    ToolsRuntime_global.h \
    VarLocker.hpp \
    aboutdlg.h \
    codeeditor/codeeditor.h \
    codeeditor/codehighlighter.h \
    codeeditor/cppcodehighlighter.h \
    codeeditor/highlighteditmodel.h \
    codeeditor/highlighterstyle.h \
    codeeditor/rslcodehighlighter.h \
    codeeditor/sqlcodehighlighter.h \
    codeeditor/xmlcodehighlighter.h \
    errordlg.h \
    errorfiltermodel_p.h \
    errorsmodel.h \
    iconlibdlg.h \
    netapi/apiuser.h \
    optionsdlg/OptionsPage.h \
    optionsdlg/codeeditoroptionspage.h \
    optionsdlg/commandsoptions.h \
    optionsdlg/commandsstorage.h \
    optionsdlg/jsonsettings.h \
    optionsdlg/logoptionspage.h \
    optionsdlg/optionsdlg.h \
    optionsdlg/rsloptionspage.h \
    optionsdlg/styleoptionspage.h \
    optionsdlg/updateoptionspage.h \
    propertywidgetmapper.h \
    rslexecutor.h \
    rslmodule/io/iobuffer.h \
    rslmodule/io/iodevice.h \
    rslmodule/io/iodir.h \
    rslmodule/io/iofile.h \
    rslmodule/io/iofileinfo.h \
    rslmodule/io/ionamespace.h \
    rslmodule/io/iotextstream.h \
    rslmodule/io/process.h \
    rslmodule/io/temporaryfile.h \
    rslmodule/iostaticmodule.h \
    rslmodule/iterableobjectbase.h \
    rslmodule/regexp/regexp.h \
    rslmodule/regexpmodule.h \
    rslmodule/richtext/rttable.h \
    rslmodule/richtext/rttablecell.h \
    rslmodule/richtext/rttextblockformat.h \
    rslmodule/richtext/rttextcharformat.h \
    rslmodule/richtext/rttextcursor.h \
    rslmodule/richtext/rttextdocument.h \
    rslmodule/richtextmodule.h \
    rslmodule/rslstringlist.h \
    rslmodule/sql/sqldatabase.h \
    rslmodule/sql/sqlquery.h \
    rslmodule/sqlstaticmodule.h \
    rslmodule/stddialogs.h \
    rslmodule/toolsruntimemodule.h \
    rslmodule/toolsruntinersl.h \
    rslmodule/uistaticmodule.h \
    rslmodule/rslstr/strlib.h \
    rslmodule/variantlist.h \
    rsscript/RslModulePluginInterface.h \
    rsscript/TRsbRSLInstTmpl.hpp \
    rsscript/addtemplproc.hpp \
    rsscript/registerinfobase.h \
    rsscript/registerobjlist.hpp \
    rsscript/rslibdynamicfuncs.h \
    rsscript/rsllistwrapper.h \
    rsscript/rslobjconnections.h \
    rsscript/rslstaticmodule.h \
    rsscript/typeinfo_p.h \
    spelling/spellchecker.h \
    spelling/spellcheckerdlg.h \
    spelling/spellstringsdlg.h \
    svn/svnstatusmodel.h \
    toolsqlconverter.h \
    toolsruntime.h \
    updatechecker.h \
    updatecheckermessagebox.h \
    widgets/codeinputdialog.h \
    widgets/doubleprogressdialog.h \
    widgets/doubleprogressdialogprivate.h \
    widgets/selectactiondlg.h \
    widgets/stringlisteditor.h \
    widgets/treeheadervew/gridtableheadermodel.h \
    widgets/treeheadervew/gridtableheaderview.h \
    widgets/treeheadervew/tableheaderitem.h \
    windowactionsregistry.h \
    xmlvalidator.h

OTHER_FILES += com.rs.worktools.runtime.xml

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    aboutdlg.ui \
    errordlg.ui \
    iconlibdlg.ui \
    optionsdlg/codeeditoroptionspage.ui \
    optionsdlg/commandsoptions.ui \
    optionsdlg/logoptionspage.ui \
    optionsdlg/optionsdlg.ui \
    optionsdlg/rsloptionspage.ui \
    optionsdlg/styleoptionspage.ui \
    optionsdlg/updateoptionspage.ui \
    spelling/spellcheckerdlg.ui \
    spelling/spellstringsdlg.ui \
    widgets/selectactiondlg.ui \
    widgets/stringlisteditor.ui

LIBS += -lVersion -lGdi32 -lUser32

RESOURCES += \
    res/iconlib.qrc \
    res/res.qrc

release: LIBS += -L./rstools/lib/release -L./hunspell/lib/release
debug: LIBS += -L./rstools/lib/release -L./hunspell/lib/debug
#debug: LIBS += -L./rstools/lib/debug -L./hunspell/lib/debug

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/release/ -lrsl_toolsruntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/debug/ -lrsl_toolsruntimed
else:unix: LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/ -lrsl_toolsruntime

LIBS += -lRSScript -lrsrtlwm -lrsldlmms -llibhunspell -lAdvapi32
#LIBS += -lrsldlmms
INCLUDEPATH += $$PWD/rstools/include $$PWD/rstools/include/panel $$PWD/rstools/include/bedit
INCLUDEPATH += $$PWD/hunspell/include

#DEFINES += BUILD_DLM
#INCLUDEPATH += D:/Build/Complect.19/Build/rstools/tools/h d:/Build/PgSQL_Main/tools/Build/panel/h d:/Build/PgSQL_Main/tools/Build/bedit/h
