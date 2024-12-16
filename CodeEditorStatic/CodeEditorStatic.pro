QT += widgets

TEMPLATE = lib
CONFIG += c++17 staticlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += TOOLSRUNTIME_LIBRARY
INCLUDEPATH += $$PWD/../ToolsRuntime

SOURCES += \
    codeeditor/codeeditor.cpp \
    codeeditor/codehighlighter.cpp \
    codeeditor/cppcodehighlighter.cpp \
    codeeditor/highlighteditmodel.cpp \
    codeeditor/highlighterstyle.cpp \
    codeeditor/rslcodehighlighter.cpp \
    codeeditor/sqlcodehighlighter.cpp \
    codeeditor/xmlcodehighlighter.cpp

HEADERS += \
    codeeditor/codeeditor.h \
    codeeditor/codehighlighter.h \
    codeeditor/cppcodehighlighter.h \
    codeeditor/highlighteditmodel.h \
    codeeditor/highlighterstyle.h \
    codeeditor/rslcodehighlighter.h \
    codeeditor/sqlcodehighlighter.h \
    codeeditor/xmlcodehighlighter.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    codeeditor.qrc
