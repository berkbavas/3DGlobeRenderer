cmake_minimum_required(VERSION 3.25.1)

project(3DGlobeRenderer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include)

set(LIBS_DIR Libs/qtimgui/lib)

set(LIBS
    debug debug/imgui
    debug debug/qt_imgui_widgets
    optimized release/imgui
    optimized release/qt_imgui_widgets
)

find_package(Qt6 COMPONENTS Core Widgets OpenGL Gui REQUIRED)

file(GLOB_RECURSE SOURCES Source/*.cpp 3DGlobeRenderer.qrc)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Source" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(3DGlobeRenderer ${SOURCES})

target_link_libraries(3DGlobeRenderer Qt6::Core Qt6::Widgets Qt6::OpenGL ${LIBS})

add_custom_command(TARGET 3DGlobeRenderer
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources/"
    "$<TARGET_FILE_DIR:3DGlobeRenderer>/Resources/"
)
	
add_custom_command(TARGET 3DGlobeRenderer
    POST_BUILD COMMAND
    Qt6::windeployqt 
    --dir "$<TARGET_FILE_DIR:3DGlobeRenderer>" 
    "$<TARGET_FILE_DIR:3DGlobeRenderer>/$<TARGET_FILE_NAME:3DGlobeRenderer>"
)
