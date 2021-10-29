cmake_minimum_required(VERSION 3.14)
project(toyt_psps_model_example VERSION 0.0.1 LANGUAGES CXX)

find_package(MVVM REQUIRED)

set(executable_name toy-psps-model)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")
add_executable(${executable_name} ${source_files} ${include_files})

target_link_libraries(${executable_name} PRIVATE Qt5::Widgets MVVM::View)

