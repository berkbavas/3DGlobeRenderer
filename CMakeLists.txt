cmake_minimum_required(VERSION 3.25.1)

project(3DEarthRenderer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt5_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include)
                
set(LIBS_DIR Libs/qtimgui/lib)

set(LIBS imgui implot qt_imgui_widgets)

find_package(Qt5 COMPONENTS Core Widgets OpenGL Gui REQUIRED)

add_compile_options(/bigobj)

file(GLOB SOURCES Source/*.cpp Source/*.h 3DEarthRenderer.qrc)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(3DEarthRenderer ${SOURCES})

target_link_libraries(3DEarthRenderer Qt5::Core Qt5::Widgets Qt5::OpenGL ${LIBS})

add_custom_command(TARGET 3DEarthRenderer
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(OutDir)resources/*\""
)

add_custom_command(TARGET 3DEarthRenderer
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(BuildDir)resources/*\""
)

add_custom_command(TARGET 3DEarthRenderer
    POST_BUILD COMMAND "$ENV{Qt5_DIR}/bin/windeployqt.exe"
    "\"$(OutDir)$(TargetName)$(TargetExt)\""
)