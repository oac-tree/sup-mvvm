cmake_minimum_required(VERSION 3.13)

project(hello_world_example VERSION 0.2.0 LANGUAGES CXX)

if(NOT TARGET sup-mvvm::view)
  find_package(sup-mvvm REQUIRED)
endif()

set(executable_name hello-world)
coa_add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets sup-mvvm::view)
set_target_properties(${executable_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUP_MVVM_EXAMPLE_RUNTIME_DIR})
