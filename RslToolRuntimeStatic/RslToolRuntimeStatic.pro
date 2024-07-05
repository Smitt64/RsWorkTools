QT += widgets

TEMPLATE = lib
CONFIG += plugin staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    rsltoolsruntimemodule.cpp \
    toolsruntimemodule.cpp

HEADERS += \
    rsltoolsruntimemodule.h \
    toolsruntimemodule.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH  += ../ToolsRuntime

DISTFILES += \
    RslToolsRuntimeModule.json
