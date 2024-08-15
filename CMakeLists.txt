cmake_minimum_required(VERSION 3.30)

project(RsWorkToolsProj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "d:/Qt/5.15.2/msvc2019/lib/cmake")

find_package(QT REQUIRED COMPONENTS Core Widgets Gui NAMES Qt5)
message("qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Widgets webengine webenginewidgets xml uitools sql)

add_subdirectory(RslTest)
add_subdirectory(RslToolRuntimeStatic)
add_subdirectory(ToolsRuntime)
add_subdirectory(WindowsModernStyle)