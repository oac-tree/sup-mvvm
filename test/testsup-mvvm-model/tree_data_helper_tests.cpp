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

#include "mvvm/serialization/tree_data_helper.h"

#include <mvvm/serialization/tree_data.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests helper methods in tree_data_helper.h.
 */
class TreeDataHelperTests : public ::testing::Test
{
};

TEST_F(TreeDataHelperTests, IsItemContainerConvertible)
{
  const tree_data_t tree_data("abc");
  EXPECT_FALSE(IsItemContainerConvertible(tree_data));
}

TEST_F(TreeDataHelperTests, IsTaggedItemsConvertible)
{
  const tree_data_t tree_data("abc");
  EXPECT_FALSE(IsTaggedItemsConvertible(tree_data));
}

TEST_F(TreeDataHelperTests, IsSessionItemConvertible)
{
  const tree_data_t tree_data("abc");
  EXPECT_FALSE(IsSessionItemConvertible(tree_data));
}

}  // namespace mvvm::test
