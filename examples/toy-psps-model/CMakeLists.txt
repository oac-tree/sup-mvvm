cmake_minimum_required(VERSION 3.14)
project(toyt_psps_model_example VERSION 0.0.1 LANGUAGES CXX)

set(executable_name toy-psps-model)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")
add_executable(${executable_name} ${source_files} ${include_files})

target_link_libraries(${executable_name} mvvm_model)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
