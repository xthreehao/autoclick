cmake_minimum_required(VERSION 3.16)
project(MyFirstApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # Qt 的元对象编译器

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(myapp main.cpp ui.cpp)
target_link_libraries(myapp Qt6::Core Qt6::Widgets)