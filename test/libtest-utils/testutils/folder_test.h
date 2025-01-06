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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_
#define LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_

#include <mvvm/test/folder_output_based_test.h>

namespace mvvm::test
{

/**
 * @brief The FolderTest class is intended for unit tests that producing some output on
 * disk.
 */
class FolderTest : public FolderOutputBasedTest
{
public:
  explicit FolderTest(std::string test_home_dirname);
};

/**
 * @brief  Returns full path to the main test folder, as defined by CMake at compile time.
 *
 * Should point to CMAKE_BINARY_DIR/test_output
 */
std::string GetTestSuiteOutputDir();

/**
 * @brief Returns full path to repository source directory.
 */
std::string GetProjectSourceDir();

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_
