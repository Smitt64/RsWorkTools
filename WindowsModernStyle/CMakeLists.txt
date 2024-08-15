file(GLOB_RECURSE WindowsModernStyle_SRC
     "./*.h" "./*.hpp" "./*.cpp" "!**/debug/**" "!**/release/**"
)

add_library(WindowsModernStyle SHARED
    ${WindowsModernStyle_SRC}
)

find_package(QT REQUIRED COMPONENTS Core Widgets Gui NAMES Qt5)

foreach (dir IN ITEMS ${Qt5Widgets_INCLUDE_DIRS})
    list(APPEND Qt5WidgetsPrivate_INCLUDE_DIRS "${dir}/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
    list(APPEND Qt5WidgetsPrivate_INCLUDE_DIRS "${dir}/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}/QtWidgets")
endforeach()

foreach (dir IN ITEMS ${Qt5Core_INCLUDE_DIRS})
list(APPEND Qt5CorePrivate_INCLUDE_DIRS "${dir}/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
    list(APPEND Qt5CorePrivate_INCLUDE_DIRS "${dir}/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}/QtCore")
endforeach()

foreach (dir IN ITEMS ${Qt5Gui_INCLUDE_DIRS})
    list(APPEND Qt5GuiPrivate_INCLUDE_DIRS "${dir}/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}/QtGui")
endforeach()

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5WidgetsPrivate_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5CorePrivate_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5GuiPrivate_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/style)

add_definitions(-DUNICODE -DQ_WS_WIN)
target_compile_definitions(WindowsModernStyle PUBLIC QT_PLUGIN)
target_link_libraries(WindowsModernStyle Version Gdi32 User32 uxtheme)

target_link_libraries(WindowsModernStyle Qt5::Widgets Qt5::Gui Qt5::Core Qt::CorePrivate Qt::WidgetsPrivate)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}) 