QT += gui widgets gui-private widgets-private

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

include(../../configtools.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    style/qwindowsvistastyle.cpp \
    style/qwindowsxpstyle.cpp \
    style/windowsmodernstyle.cpp \
    windowsmodernstyleplugin.cpp

HEADERS += \
    style/qwindowsvistastyle_p.h \
    style/qwindowsvistastyle_p_p.h \
    style/qwindowsxpstyle_p.h \
    style/qwindowsxpstyle_p_p.h \
    style/windowsmodernstyle.h \
    windowsmodernstyleplugin.h

DISTFILES += WindowsModernStyle.json

QMAKE_USE_PRIVATE += user32 gdi32
LIBS_PRIVATE *= -luxtheme

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/styles
}
!isEmpty(target.path): INSTALLS += target
