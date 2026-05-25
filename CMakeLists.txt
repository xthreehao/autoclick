cmake_minimum_required(VERSION 3.16)
project(MyFirstApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets DBus)

add_executable(myapp main.cpp ui.cpp log.cpp MainWindow.cpp MyCustomsSlider.cpp clicker.cpp keyboard_monitor.cpp keyboard_monitor.h head.h)

target_link_libraries(myapp
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::DBus
)