cmake_minimum_required(VERSION 3.13.5)
project(sup-mvvm-project VERSION 0.4.99 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(SUP_MVVM_BUMP_VERSION "Propagate version number" OFF)
option(SUP_MVVM_EXPORT_BUILDTREE "Export build tree in /home/user/.cmake registry" OFF)
option(SUP_MVVM_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(SUP_MVVM_SETUP_COVERAGE "Setups target for code coverage" OFF)
option(SUP_MVVM_USE_QT6 "Compile with Qt6" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(source)
add_subdirectory(extern)
add_subdirectory(tests)
add_subdirectory(doc)

include(installation)
add_subdirectory(examples)

project_codetools_setup()
