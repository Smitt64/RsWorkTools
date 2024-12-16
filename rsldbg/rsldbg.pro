QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += RSLDBG_LIBRARY TOOLSRUNTIME_STATIC_LINK

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cdebug.cpp \
    cdebugroot.cpp \
    clocals.cpp \
    convert.cpp \
    cqsurvey.cpp \
    csurvey.cpp \
    cwatchv.cpp \
    displayvar.cpp \
    lf/appcfgdata2.cpp \
    lf/dbgexception.cpp \
    rembreakpoints.cpp \
    remexpressions.cpp \
    rsldbg.cpp \
    saver.cpp \
    ui/events/dbgbreakpointevent.cpp \
    ui/events/dbgtraceevent.cpp \
    ui/mainwindow.cpp

HEADERS += \
    cdebug.h \
    cdebugroot.h \
    clocals.h \
    convert.h \
    cqsurvey.h \
    csurvey.h \
    cwatchv.h \
    dbg.h \
    displayvar.h \
    lf/bp_data.h \
    lf/dbgexception.h \
    lf/types.h \
    rembreakpoints.h \
    remexpressions.h \
    rsldbg_global.h \
    rsldbg.h \
    saver.h \
    tracemsg.h \
    ui/events/dbgbreakpointevent.h \
    ui/events/dbgtraceevent.h \
    ui/mainwindow.h

INCLUDEPATH += $$PWD/../ToolsRuntime

# Default rules for deployment.
target.path = $$PWD/../RslTest/debug
INSTALLS += target

release: LIBS += -L../ToolsRuntime/rstools/lib/release
debug: LIBS += -L../ToolsRuntime/rstools/lib/release
INCLUDEPATH += $$PWD/rstools/include $$PWD/rstools/include/panel $$PWD/rstools/include/bedit

LIBS += -lRSScript -lrsrtlwm -lrsldlmms -lUser32 -lShell32
INCLUDEPATH += $$PWD/../ToolsRuntime/rstools/include $$PWD/../ToolsRuntime/rstools/include/panel $$PWD/../ToolsRuntime/rstools/include/bedit

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../loki/release/ -lloki
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../loki/debug/ -lloki
else:unix: LIBS += -L$$OUT_PWD/../loki/ -lloki

INCLUDEPATH += $$PWD/../loki
DEPENDPATH += $$PWD/../loki

LIBS += -lVersion -lGdi32 -lUser32 -lAdvapi32

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../loki/release/libloki.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../loki/debug/libloki.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../loki/release/loki.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../loki/debug/loki.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../loki/libloki.a

FORMS += \
    ui/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CodeEditorStatic/release/ -lCodeEditorStatic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CodeEditorStatic/debug/ -lCodeEditorStatic
else:unix: LIBS += -L$$OUT_PWD/../CodeEditorStatic/ -lCodeEditorStatic

INCLUDEPATH += $$PWD/../CodeEditorStatic
DEPENDPATH += $$PWD/../CodeEditorStatic

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CodeEditorStatic/release/libCodeEditorStatic.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CodeEditorStatic/debug/libCodeEditorStatic.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CodeEditorStatic/release/CodeEditorStatic.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CodeEditorStatic/debug/CodeEditorStatic.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../CodeEditorStatic/libCodeEditorStatic.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CodeEditorStatic/release/ -lCodeEditorStatic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CodeEditorStatic/debug/ -lCodeEditorStatic
else:unix: LIBS += -L$$OUT_PWD/../CodeEditorStatic/ -lCodeEditorStatic

INCLUDEPATH += $$PWD/../CodeEditorStatic
DEPENDPATH += $$PWD/../CodeEditorStatic
