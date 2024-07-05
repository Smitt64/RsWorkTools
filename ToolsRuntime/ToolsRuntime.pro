QT += gui widgets webengine webenginewidgets xml

TEMPLATE = lib
DEFINES += TOOLSRUNTIME_LIBRARY

CONFIG += c++17
RC_FILE = res/winfmt.rc

include(../../configtools.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdlg.cpp \
    errorsmodel.cpp \
    rslexecutor.cpp \
    rsscript/Properties.cpp \
    rsscript/registerinfobase.cpp \
    rsscript/registerobjlist.cpp \
    rsscript/rsllistitemcontainer.cpp \
    rsscript/rsllistwrapper.cpp \
    rsscript/rslstaticmodule.cpp \
    rsscript/typeinfofunc.cpp \
    toolsruntime.cpp \
    updatechecker.cpp \
    updatecheckermessagebox.cpp

HEADERS += \
    ToolsRuntime_global.h \
    aboutdlg.h \
    errorsmodel.h \
    rslexecutor.h \
    rsscript/TRsbRSLInstTmpl.hpp \
    rsscript/registerinfobase.h \
    rsscript/registerobjlist.hpp \
    rsscript/rsllistwrapper.h \
    rsscript/rslstaticmodule.h \
    rsscript/typeinfo_p.h \
    toolsruntime.h \
    updatechecker.h \
    updatecheckermessagebox.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    aboutdlg.ui

LIBS += -lVersion

RESOURCES += \
    res/res.qrc

release: LIBS += -L./rstools/lib/release
debug: LIBS += -L./rstools/lib/debug

LIBS += -lRSScript -lrsrtlwm -lrsldlmms
#LIBS += -lrsldlmms
INCLUDEPATH += $$PWD/rstools/include $$PWD/rstools/include/panel $$PWD/rstools/include/bedit

#DEFINES += BUILD_DLM
#INCLUDEPATH += D:/Build/Complect.19/Build/rstools/tools/h d:/Build/PgSQL_Main/tools/Build/panel/h d:/Build/PgSQL_Main/tools/Build/bedit/h
