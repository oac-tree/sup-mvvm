cmake_minimum_required(VERSION 3.13.5)
project(cpp-mvvm VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(MVVM_BUMP_VERSION "Propagate version number" OFF)
option(MVVM_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(MVVM_SETUP_COVERAGE "Setups target for code coverage" OFF)
option(MVVM_USE_QT6 "Compile with Qt6" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(source)
add_subdirectory(extern)
add_subdirectory(tests)

include(installation)
add_subdirectory(examples)

project_codetools_setup()
