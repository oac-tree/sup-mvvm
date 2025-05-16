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

#ifndef LIBTEST_UTILS_TESTUTILS_MODEL_TEST_HELPER_H_
#define LIBTEST_UTILS_TESTUTILS_MODEL_TEST_HELPER_H_

//! @file
//! Collection of helper function for model testing.

#include <mvvm/model/path.h>

#include <memory>
#include <string>

namespace mvvm
{
class SessionItem;
class ISessionModel;
}  // namespace mvvm

namespace mvvm::test
{

//! Holds information about item.
struct ItemInfo
{
  SessionItem* item{nullptr};
  std::string identifier;
  Path path;
};

//! Model and covenience vector of items populating the model.
struct ItemTreeData
{
  std::unique_ptr<ISessionModel> model;
  std::vector<ItemInfo> item_info;
};

std::unique_ptr<ItemTreeData> CreateTestData(std::size_t max_depth_level,
                                             std::size_t children_per_node);

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MODEL_TEST_HELPER_H_
