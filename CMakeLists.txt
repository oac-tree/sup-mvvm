# Project       : Operational Applications UI Foundation
#
# Description   : The model-view-viewmodel library of generic UI components
#
# Author        : Gennady Pospelov (IO)
#
# Copyright (c) : 2010-2025 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.

cmake_minimum_required(VERSION 3.13...3.31)
project(sup-mvvm-project VERSION 1.5.0 LANGUAGES CXX)

option(COA_COVERAGE "Generate unit test coverage information" OFF)
option(COA_PARASOFT_INTEGRATION "Parasoft integration" OFF)
option(COA_EXPORT_BUILD_TREE "Export build tree in /home/user/.cmake registry" OFF)
option(COA_BUILD_TESTS "Build unit tests" ON)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(COA_NO_CODAC "Don't look for the presence of CODAC environment" OFF)
option(COA_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(COA_USE_QT6 "Compile with Qt6" ON)
option(SUP_MVVM_BUILD_EXAMPLES "Compile example projects" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(src)
add_subdirectory(extern)
add_subdirectory(test)
add_subdirectory(doc)
add_subdirectory(examples)

include(installation)

project_codetools_setup()
