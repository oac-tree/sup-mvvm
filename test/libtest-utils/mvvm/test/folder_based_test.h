/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_
#define LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_

#include <mvvm/test/folder_output_based_test.h>
#include <mvvm/test/test_utils.h>

namespace mvvm::test
{

/**
 * @brief The FolderBasedTest class is intended for unit tests that producing some output on
 * disk.
 */

class FolderBasedTest : public FolderOutputBasedTest
{
public:
  explicit FolderBasedTest(std::string test_home_dirname)
      : FolderOutputBasedTest(GetTestSuiteOutputDir(), std::move(test_home_dirname))
  {
  }
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_
