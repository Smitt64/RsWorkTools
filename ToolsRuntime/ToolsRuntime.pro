QT += gui widgets webengine webenginewidgets xml uitools sql testlib

TEMPLATE = lib
DEFINES += TOOLSRUNTIME_LIBRARY Q_WS_WIN

CONFIG += c++17
RC_FILE = res/winfmt.rc

include(../../configtools.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
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
    optionsdlg/OptionsPage.cpp \
    optionsdlg/codeeditoroptionspage.cpp \
    optionsdlg/commandsoptions.cpp \
    optionsdlg/commandsstorage.cpp \
    optionsdlg/jsonsettings.cpp \
    optionsdlg/logoptionspage.cpp \
    optionsdlg/optionsdlg.cpp \
    optionsdlg/rsloptionspage.cpp \
    optionsdlg/styleoptionspage.cpp \
    rslexecutor.cpp \
    rslmodule/dialogs/message_box.cpp \
    rslmodule/io/iobuffer.cpp \
    rslmodule/io/iodevice.cpp \
    rslmodule/io/iofile.cpp \
    rslmodule/io/iotextstream.cpp \
    rslmodule/iostaticmodule.cpp \
    rslmodule/regexp/regexp.cpp \
    rslmodule/regexpmodule.cpp \
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
    toolsruntime.cpp \
    updatechecker.cpp \
    updatecheckermessagebox.cpp \
    widgets/selectactiondlg.cpp \
    widgets/stringlisteditor.cpp \
    windowactionsregistry.cpp

HEADERS += \
    ToolsRuntime_global.h \
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
    optionsdlg/OptionsPage.h \
    optionsdlg/codeeditoroptionspage.h \
    optionsdlg/commandsoptions.h \
    optionsdlg/commandsstorage.h \
    optionsdlg/jsonsettings.h \
    optionsdlg/logoptionspage.h \
    optionsdlg/optionsdlg.h \
    optionsdlg/rsloptionspage.h \
    optionsdlg/styleoptionspage.h \
    rslexecutor.h \
    rslmodule/io/iobuffer.h \
    rslmodule/io/iodevice.h \
    rslmodule/io/iofile.h \
    rslmodule/io/iotextstream.h \
    rslmodule/iostaticmodule.h \
    rslmodule/regexp/regexp.h \
    rslmodule/regexpmodule.h \
    rslmodule/sql/sqldatabase.h \
    rslmodule/sql/sqlquery.h \
    rslmodule/sqlstaticmodule.h \
    rslmodule/stddialogs.h \
    rslmodule/toolsruntimemodule.h \
    rslmodule/toolsruntinersl.h \
    rslmodule/uistaticmodule.h \
    rsscript/RslModulePluginInterface.h \
    rsscript/TRsbRSLInstTmpl.hpp \
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
    toolsruntime.h \
    updatechecker.h \
    updatecheckermessagebox.h \
    widgets/selectactiondlg.h \
    widgets/stringlisteditor.h \
    windowactionsregistry.h

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
    spelling/spellcheckerdlg.ui \
    spelling/spellstringsdlg.ui \
    widgets/selectactiondlg.ui \
    widgets/stringlisteditor.ui

LIBS += -lVersion -lGdi32 -lUser32

RESOURCES += \
    res/iconlib.qrc \
    res/res.qrc

release: LIBS += -L./rstools/lib/release -L./hunspell/lib/release
debug: LIBS += -L./rstools/lib/debug -L./hunspell/lib/debug

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/release/ -lrsl_toolsruntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/debug/ -lrsl_toolsruntimed
else:unix: LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/ -lrsl_toolsruntime

LIBS += -lRSScript -lrsrtlwm -lrsldlmms -llibhunspell
#LIBS += -lrsldlmms
INCLUDEPATH += $$PWD/rstools/include $$PWD/rstools/include/panel $$PWD/rstools/include/bedit
INCLUDEPATH += $$PWD/hunspell/include

#DEFINES += BUILD_DLM
#INCLUDEPATH += D:/Build/Complect.19/Build/rstools/tools/h d:/Build/PgSQL_Main/tools/Build/panel/h d:/Build/PgSQL_Main/tools/Build/bedit/h
