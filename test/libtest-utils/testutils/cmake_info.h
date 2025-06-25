/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTUTILS_CMAKE_INFO_H_
#define TESTUTILS_CMAKE_INFO_H_

#include <string>

namespace mvvm::test
{

/**
 * @brief Returns the full path to the source directory.
 */
std::string CMakeSourceDir();

/**
 * @brief Returns full path to the build directory.
 */
std::string CMakeBinaryDir();

/**
 * @brief Returns full path to the folder with test results.
 *
 * Normally points to CMAKE_CURRENT_BINARY_DIR/test_output
 */
std::string TestOutputDir();

/**
 * @brief Returns full path to the project source directory.
 *
 * Can be different from CMake source directory, if the project is in a submodule.
 */
std::string ProjectSourceDir();

}  // namespace mvvm::test

#endif  // TESTUTILS_CMAKE_INFO_H_
