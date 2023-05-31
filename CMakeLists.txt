cmake_minimum_required(VERSION 3.24)
project(Optocus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGLWidgets
        OpenGL
        REQUIRED)

find_package(GLUT REQUIRED)

add_subdirectory(lib/calculating)

file(GLOB_RECURSE LIB_INCLUDE_DIRS LIST_DIRECTORIES true
        ${CMAKE_SOURCE_DIR}/lib/*/include)

include_directories(Optocus ${LIB_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(Optocus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt6::OpenGLWidgets
        OpenGL::GL
        GLUT::GLUT
        calculating
        )

if (WIN32)
    target_link_libraries(Optocus opengl32)
endif()