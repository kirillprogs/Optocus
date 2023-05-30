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

include_directories(Optocus include)
add_executable(Optocus src/main.cpp
        include/calculating/Lens.h
        include/calculating/OpticalSystem.h
        include/calculating/Object.h
        include/calculating/geometry/Point.h
        include/calculating/geometry/Ray.h
        src/calculating/Lens.cpp
        src/calculating/Object.cpp
        src/calculating/geometry/Ray.cpp
        include/calculating/OpticalDevice.h
        include/calculating/geometry/Segment.h
        src/calculating/geometry/Segment.cpp
        include/calculating/Image.h
        src/calculating/Image.cpp
        src/calculating/geometry/Point.cpp
        src/calculating/OpticalSystem.cpp
        src/pages/gui_logic/pages.cpp
        src/pages/gui_logic/pages.h
        src/pages/gui_logic/Pages.ui
        src/pages/gui_logic/pages.h
        src/pages/gui_logic/pages.h
        src/pages/HomePageWidget.cpp
        src/pages/HomePageWidget.h
        src/pages/ModelingPageWidget.cpp
        src/pages/ModelingPageWidget.h
        src/pages/CalculatorPageWidget.cpp
        src/pages/CalculatorPageWidget.h
        src/pages/DevicesPageWidget.cpp
        src/pages/DevicesPageWidget.h
        src/pages/PhenomPageWidget.cpp
        src/pages/PhenomPageWidget.h
        src/style/OptStyle.cpp
        src/style/OptStyle.h
        src/graphics/GraphicsPanel.cpp
        src/graphics/GraphicsPanel.h)

target_link_libraries(Optocus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt6::OpenGLWidgets
        opengl32
        )

include_directories(Rectangle test/src test/include)
add_executable(Rectangle test/src/rectangle.cpp test/include/rectangle.h)
target_link_libraries(Rectangle
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )