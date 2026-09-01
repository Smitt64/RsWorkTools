QT += core gui widgets network webengine webenginewidgets

TARGET = MarkdownViewer
TEMPLATE = app

include(../../configtools.pri)

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    markdownoptionspage.cpp \
    markdownsearchwidget.cpp \
    markdownsettings.cpp \
    markdownview.cpp

HEADERS += \
    mainwindow.h \
    markdownoptionspage.h \
    markdownsearchwidget.h \
    markdownsettings.h \
    markdownview.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntime/ -lToolsRuntime

LIBS += -lOle32

INCLUDEPATH += $$PWD/../ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntime

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/markdownviewer.qrc

win32:RC_FILE = res/app-icon.rc
