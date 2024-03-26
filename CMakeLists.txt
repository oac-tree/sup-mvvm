# Project       : Operational Applications UI Foundation
#
# Description   : The model-view-viewmodel library of generic UI components
#
# Author        : Gennady Pospelov (IO)
#
# Copyright (c) : 2010-2024 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.

# Set required CMake version
# This sets the default CMake behaviour through policies allowing compatibility with future versions
# Justification:
#   - CMake 3.12 - CMP0074: find_package command now searches prefixes specified by the <PackageName>_ROOT environment variable
#       CODAC systems contain conflicting environment variables which generate warnings when this policy is not set to NEW
#   - CMake 3.13 - CMP0076: relative source file paths are interpreted as being relative to the current source directory
cmake_minimum_required(VERSION 3.13)

project(sup-mvvm-project VERSION 1.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(COA_EXPORT_BUILD_TREE "Export build tree in /home/user/.cmake registry" OFF)
option(COA_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(COA_SETUP_COVERAGE "Setups target for code coverage" OFF)
option(COA_USE_QT6 "Compile with Qt6" OFF)
option(SUP_MVVM_BUILD_EXAMPLES "Compile example projects" OFF)
option(COA_BUILD_TESTS "Build unit tests" ON)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(COA_NO_CODAC "Don't look for the presence of CODAC environment" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(src)
add_subdirectory(extern)

if(COA_BUILD_TESTS)
  add_subdirectory(test)
endif()

add_subdirectory(doc)

include(installation)

if(SUP_MVVM_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

project_codetools_setup()
