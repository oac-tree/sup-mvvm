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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_CONTAINER_HELPER_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_CONTAINER_HELPER_H_

//! @file test_utils.h
//! Collection of utility functions for various unit tests.

#include <algorithm>
#include <memory>
#include <vector>

namespace mvvm::test
{

/**
 * @brief Creates vector of unique_ptr of given type.
 */
template <typename B, typename D>
auto CreateRow(int ncolumns)
{
  std::vector<std::unique_ptr<B>> result;
  for (int i = 0; i < ncolumns; ++i)
  {
    result.emplace_back(std::make_unique<D>());
  }
  return result;
}

/**
 * @brief Creates a pair of unique_ptr and raw ptr to the object of given type.
 */
template <typename T, class... Args>
auto CreateTestData(Args&&... args)
{
  auto uptr = std::make_unique<T>(std::forward<Args>(args)...);
  auto raw_ptr = uptr.get();
  return std::make_pair(std::move(uptr), raw_ptr);
}

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_CONTAINER_HELPER_H_
