cmake_minimum_required(VERSION 3.25.1)

project(3DEarthRenderer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include)    
set(LIBS_DIR Libs/qtimgui/lib)
set(DEBUG_LIBS imguid qt_imgui_widgetsd)
set(RELEASE_LIBS imgui qt_imgui_widgets)

find_package(Qt6 COMPONENTS Core Widgets OpenGL Gui REQUIRED)

add_compile_options(/bigobj)

file(GLOB_RECURSE SOURCES Source/*.cpp Source/*.h 3DEarthRenderer.qrc)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Source" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(3DEarthRenderer ${SOURCES})

target_link_libraries(3DEarthRenderer Qt6::Core Qt6::Widgets Qt6::OpenGL ${RELEASE_LIBS})

add_custom_command(TARGET 3DEarthRenderer 
POST_BUILD COMMAND xcopy /E /Y "\"$(SolutionDir)../Resources\"" "\"$(OutDir)Resources/*\"")

add_custom_command(TARGET 3DEarthRenderer
POST_BUILD COMMAND xcopy /E /Y  "\"$(SolutionDir)../Resources\"" "\"$(BuildDir)Resources/*\"")

add_custom_command(TARGET 3DEarthRenderer 
POST_BUILD COMMAND "$ENV{Qt6_DIR}/bin/windeployqt.exe" "\"$(OutDir)$(TargetName)$(TargetExt)\"")