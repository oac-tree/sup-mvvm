cmake_minimum_required(VERSION 3.13)

project(concurrentplot_example VERSION 0.0.1 LANGUAGES CXX)

if(NOT TARGET sup-mvvm::view)
  find_package(sup-mvvm REQUIRED)
endif()

find_package(Threads)

set(executable_name concurrentplot)
add_subdirectory(concurrentplotcore)

coa_add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} PRIVATE  pthread concurrentplotcore Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
