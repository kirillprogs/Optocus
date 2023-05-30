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

include_directories(Optocus include /usr/include/)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/include/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Optocus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt6::OpenGLWidgets
        OpenGL::GL
        GLUT::GLUT
        )

if (WIN32)
    target_link_libraries(Octopus opengl32)
endif()