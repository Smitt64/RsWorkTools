file(GLOB_RECURSE ToolsRuntime_SRC
     "./*.h" "./*.hpp" "./*.cpp" "!**/debug/**" "!**/release/**"
)

list(FILTER ToolsRuntime_SRC EXCLUDE REGEX ".*rstools\\/include.*")
list(FILTER ToolsRuntime_SRC EXCLUDE REGEX ".*debug.*")
list(FILTER ToolsRuntime_SRC EXCLUDE REGEX ".*release.*")

add_library(ToolsRuntime SHARED
    ${ToolsRuntime_SRC}
)

target_sources(ToolsRuntime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/res/winfmt.rc)

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5WebEngine_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5WebEngineWidgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Xml_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5UiTools_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Sql_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/rstools/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/rstools/include/panel)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/rstools/include/bedit)

target_link_directories(ToolsRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rstools/lib/debug)

target_link_libraries(ToolsRuntime Version Gdi32 User32)
target_link_libraries(ToolsRuntime RSScript rsrtlwm rsldlmms)
target_link_libraries(ToolsRuntime ToolsRuntimeStatic)
target_link_libraries(ToolsRuntime Qt5::Widgets Qt5::Core Qt5::WebEngine Qt5::WebEngineWidgets Qt5::Xml Qt5::Sql Qt5::UiTools)

add_definitions(-DTOOLSRUNTIME_LIBRARY -DQ_WS_WIN)
include_directories(${CMAKE_CURRENT_LIST_DIR})