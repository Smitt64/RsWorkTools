QT += gui widgets webengine webenginewidgets xml uitools sql

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
    errordlg.cpp \
    errorsmodel.cpp \
    optionsdlg/OptionsPage.cpp \
    optionsdlg/codeeditoroptionspage.cpp \
    optionsdlg/optionsdlg.cpp \
    optionsdlg/rsloptionspage.cpp \
    optionsdlg/styleoptionspage.cpp \
    rslexecutor.cpp \
    rslmodule/dialogs/message_box.cpp \
    rslmodule/regexp/regexp.cpp \
    rslmodule/regexpmodule.cpp \
    rslmodule/sql/sqldatabase.cpp \
    rslmodule/sql/sqlquery.cpp \
    rslmodule/sqlstaticmodule.cpp \
    rslmodule/stddialogs.cpp \
    rslmodule/toolsruntimemodule.cpp \
    rslmodule/toolsruntinersl.cpp \
    rsscript/Methods.cpp \
    rsscript/Properties.cpp \
    rsscript/registerinfobase.cpp \
    rsscript/registerobjlist.cpp \
    rsscript/rsllistitemcontainer.cpp \
    rsscript/rsllistwrapper.cpp \
    rsscript/rslstaticmodule.cpp \
    rsscript/typeinfofunc.cpp \
    toolsruntime.cpp \
    updatechecker.cpp \
    updatecheckermessagebox.cpp \
    widgets/stringlisteditor.cpp

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
    errordlg.h \
    errorfiltermodel_p.h \
    errorsmodel.h \
    optionsdlg/OptionsPage.h \
    optionsdlg/codeeditoroptionspage.h \
    optionsdlg/optionsdlg.h \
    optionsdlg/rsloptionspage.h \
    optionsdlg/styleoptionspage.h \
    rslexecutor.h \
    rslmodule/regexp/regexp.h \
    rslmodule/regexpmodule.h \
    rslmodule/sql/sqldatabase.h \
    rslmodule/sql/sqlquery.h \
    rslmodule/sqlstaticmodule.h \
    rslmodule/stddialogs.h \
    rslmodule/toolsruntimemodule.h \
    rslmodule/toolsruntinersl.h \
    rsscript/RslModulePluginInterface.h \
    rsscript/TRsbRSLInstTmpl.hpp \
    rsscript/registerinfobase.h \
    rsscript/registerobjlist.hpp \
    rsscript/rslibdynamicfuncs.h \
    rsscript/rsllistwrapper.h \
    rsscript/rslstaticmodule.h \
    rsscript/typeinfo_p.h \
    toolsruntime.h \
    updatechecker.h \
    updatecheckermessagebox.h \
    widgets/stringlisteditor.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    aboutdlg.ui \
    errordlg.ui \
    optionsdlg/codeeditoroptionspage.ui \
    optionsdlg/optionsdlg.ui \
    optionsdlg/rsloptionspage.ui \
    optionsdlg/styleoptionspage.ui \
    widgets/stringlisteditor.ui

LIBS += -lVersion -lGdi32 -lUser32

RESOURCES += \
    res/res.qrc

release: LIBS += -L./rstools/lib/release
debug: LIBS += -L./rstools/lib/debug

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/release/ -lrsl_toolsruntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/debug/ -lrsl_toolsruntimed
else:unix: LIBS += -L$$OUT_PWD/../RslToolRuntimeStatic/ -lrsl_toolsruntime
LIBS += -lRSScript -lrsrtlwm -lrsldlmms
#LIBS += -lrsldlmms
INCLUDEPATH += $$PWD/rstools/include $$PWD/rstools/include/panel $$PWD/rstools/include/bedit

#DEFINES += BUILD_DLM
#INCLUDEPATH += D:/Build/Complect.19/Build/rstools/tools/h d:/Build/PgSQL_Main/tools/Build/panel/h d:/Build/PgSQL_Main/tools/Build/bedit/h
