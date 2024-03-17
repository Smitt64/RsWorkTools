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
    toolsruntime.cpp

HEADERS += \
    ToolsRuntime_global.h \
    aboutdlg.h \
    toolsruntime.h

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
