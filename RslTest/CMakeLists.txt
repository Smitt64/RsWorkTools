file(GLOB_RECURSE RslTest_SRC "./*.h" "./*.hpp" "./*.cpp" "!**/debug/**" "!**/release/**")

add_executable(RslTest ${RslTest_SRC})

find_package(QT REQUIRED COMPONENTS Core Widgets Gui NAMES Qt5)

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})

target_link_libraries(RslTest ToolsRuntime)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../ToolsRuntime/)

target_link_libraries(RslTest Qt5::Widgets Qt5::Gui Qt5::Core Qt::CorePrivate Qt::WidgetsPrivate)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}) 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})