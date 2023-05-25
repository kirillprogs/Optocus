cmake_minimum_required(VERSION 3.24)
project(Optocus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/kirill/Qt/6.5.0/gcc_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(Optocus include)
add_executable(Optocus src/main.cpp src/Window.cpp include/Window.h)
target_link_libraries(Optocus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

include_directories(Rectangle test/src test/include)
add_executable(Rectangle test/src/rectangle.cpp)
target_link_libraries(Rectangle
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )