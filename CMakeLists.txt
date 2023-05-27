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
add_executable(Optocus src/main.cpp
        include/calculating/Lens.h
        include/calculating/OpticalSystem.h
        include/calculating/Object.h
        include/calculating/Point.h
        include/calculating/Ray.h
        src/calculating/Lens.cpp
        src/calculating/Object.cpp
        src/calculating/Ray.cpp
        src/calculating/Point.cpp
        include/calculating/OpticalDevice.h src/Window.cpp
        include/calculating/Segment.h src/calculating/Segment.cpp include/calculating/Image.h src/calculating/Image.cpp)

target_link_libraries(Optocus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

include_directories(Rectangle test/src test/include)
add_executable(Rectangle test/src/rectangle.cpp test/include/rectangle.h)
target_link_libraries(Rectangle
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )