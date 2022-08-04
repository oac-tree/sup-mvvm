cmake_minimum_required(VERSION 3.13.5)
project(hello_world_example VERSION 0.0.1 LANGUAGES CXX)

find_package(MVVM REQUIRED)

set(executable_name hello-world)
add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets MVVM::View)
