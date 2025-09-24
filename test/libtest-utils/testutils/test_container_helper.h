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
auto CreateUnique(Args&&... args)
{
  auto uptr = std::make_unique<T>(std::forward<Args>(args)...);
  auto raw_ptr = uptr.get();
  return std::make_pair(std::move(uptr), raw_ptr);
}

/**
 * @brief Checks if vectors are of the same size and contain the same elements (order is not
 * important).
 */
template <typename VectorT>
bool HaveSameElements(const VectorT& vec1, const VectorT& vec2)
{
  if (vec1.size() != vec2.size())
  {
    return false;
  }

  VectorT sorted1 = vec1;
  VectorT sorted2 = vec2;

  std::sort(sorted1.begin(), sorted1.end());
  std::sort(sorted2.begin(), sorted2.end());

  return sorted1 == sorted2;
}

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_CONTAINER_HELPER_H_
