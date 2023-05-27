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
add_executable(Optocus src/main.cpp src/Window.cpp include/Window.h src/pages/gui_logic/pages.cpp src/pages/gui_logic/pages.h src/pages/gui_logic/Pages.ui src/pages/gui_logic/pages.h src/pages/gui_logic/pages.h src/pages/HomePageWidget.cpp src/pages/HomePageWidget.h src/pages/ModelingPageWidget.cpp src/pages/ModelingPageWidget.h src/pages/CalculatorPageWidget.cpp src/pages/CalculatorPageWidget.h src/pages/DevicesPageWidget.cpp src/pages/DevicesPageWidget.h src/pages/PhenomPageWidget.cpp src/pages/PhenomPageWidget.h src/style/OptStyle.cpp src/style/OptStyle.h)
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