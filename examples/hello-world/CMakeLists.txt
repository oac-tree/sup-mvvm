cmake_minimum_required(VERSION 3.14)
project(hello_world_example VERSION 0.0.1 LANGUAGES CXX)

set(executable_name hello-world)
add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} mvvm_model)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
