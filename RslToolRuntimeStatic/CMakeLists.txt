set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ToolsRuntimeStatic_SRC
     "*.h"
     "*.hpp"
     "*.cpp"
)

add_library(ToolsRuntimeStatic STATIC
    ${ToolsRuntimeStatic_SRC}
)

target_compile_definitions(ToolsRuntimeStatic PUBLIC QT_STATICPLUGIN)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../ToolsRuntime)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Sql_INCLUDE_DIRS})

target_link_libraries(ToolsRuntimeStatic Qt5::Widgets Qt5::Core)

include_directories(${CMAKE_CURRENT_LIST_DIR})