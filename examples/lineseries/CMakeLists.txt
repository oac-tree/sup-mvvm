cmake_minimum_required(VERSION 3.13)

project(lineseries_example VERSION 0.2.0 LANGUAGES CXX)

if(NOT TARGET sup-mvvm::view)
  find_package(sup-mvvm REQUIRED)
endif()

set(executable_name lineseries)
add_subdirectory(lineseriescore)

coa_add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} PRIVATE lineseriescore Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
set_target_properties(${executable_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUP_MVVM_EXAMPLE_RUNTIME_DIR})
