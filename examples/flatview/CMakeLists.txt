cmake_minimum_required(VERSION 3.13.5)
project(flatview_example VERSION 0.2.0 LANGUAGES CXX)

find_package(sup-mvvm REQUIRED)

set(executable_name flatview)
add_subdirectory(flatviewcore)
add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} flatviewcore Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
