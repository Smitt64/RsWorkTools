QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RibbonMacroTest
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntime/ -lToolsRuntime

LIBS += -lOle32

INCLUDEPATH += $$PWD/../ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntime
