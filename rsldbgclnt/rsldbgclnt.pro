QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbgeditorlinewidgetprovider.cpp \
    logevent.cpp \
    main.cpp \
    mainwindow.cpp \
    models/callstackmodel.cpp \
    models/logeventmodel.cpp \
    models/varwatchmodel.cpp \
    stdviewdockwidget.cpp \
    varwatchdlg.cpp \
    varwatchdockwidget.cpp

HEADERS += \
    ../rsldbg/dbgserverproto.h \
    dbgeditorlinewidgetprovider.h \
    logevent.h \
    mainwindow.h \
    models/callstackmodel.h \
    models/logeventmodel.h \
    models/varwatchmodel.h \
    stdviewdockwidget.h \
    varwatchdlg.h \
    varwatchdockwidget.h

FORMS += \
    mainwindow.ui \
    varwatchdlg.ui

INCLUDEPATH += $$PWD/../rsldbg
INCLUDEPATH += $$PWD/../ToolsRuntime
INCLUDEPATH += $$PWD/../CodeEditorStatic

DEPENDPATH += $$PWD/../CodeEditorStatic
LIBS += -lUser32

target.path = $$PWD/../RslTest/debug
INSTALLS += target

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

RESOURCES += \
    dbgres.qrc \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FindDialog/release/ -lFindDialog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FindDialog/debug/ -lFindDialog
else:unix: LIBS += -L$$OUT_PWD/../FindDialog/ -lFindDialog

INCLUDEPATH += $$PWD/../FindDialog
DEPENDPATH += $$PWD/../FindDialog

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FindDialog/release/libFindDialog.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FindDialog/debug/libFindDialog.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FindDialog/release/FindDialog.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FindDialog/debug/FindDialog.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../FindDialog/libFindDialog.a
