cmake_minimum_required(VERSION 3.16)
project(MyFirstApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets DBus)
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBus REQUIRED dbus-1)

add_executable(myapp main.cpp ui.cpp log.cpp MyCustomsSlider.cpp clicker.cpp ghotkey.cpp hotkey_registrar.cpp hotkey_registrar.h)

# 一次性链接所有需要的库
target_link_libraries(myapp
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::DBus
    ${DBus_LIBRARIES}
)

# 添加 DBus 头文件路径
target_include_directories(myapp PRIVATE ${DBus_INCLUDE_DIRS})