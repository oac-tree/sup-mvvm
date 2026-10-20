/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/providers/presentation_item_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for helper methods from presentation_item_helper.h
 */
class PresentationItemHelperTest : public ::testing::Test
{
};

TEST_F(PresentationItemHelperTest, GetItemDataDisplayRole)
{
  const role_t main_data_role = DataRole::kData;

  {  // boolean True
    SessionItem item;
    item.SetData(true, main_data_role);
    EXPECT_EQ(GetItemDataDisplayRole(item, main_data_role), QString("True"));
  }

  {  // int
    SessionItem item;
    item.SetData(42, main_data_role);
    EXPECT_EQ(GetItemDataDisplayRole(item, main_data_role), QString("42"));
  }

  {  // float64
    SessionItem item;
    item.SetData(mvvm::float64{42.123456789}, main_data_role);
    EXPECT_EQ(GetItemDataDisplayRole(item, main_data_role), "42.1235");
  }

  {  // string
    SessionItem item;
    item.SetData(std::string("abc"), main_data_role);
    EXPECT_EQ(GetItemDataDisplayRole(item, main_data_role), "abc");
  }

  // more tests in variant_display_role_visitor_tests.cpp
}

TEST_F(PresentationItemHelperTest, GetQtRole)
{
  EXPECT_EQ(GetQtRole(Qt::DisplayRole), static_cast<role_t>(Qt::DisplayRole));
  EXPECT_EQ(GetQtRole(Qt::EditRole), static_cast<role_t>(Qt::EditRole));
  EXPECT_EQ(GetQtRole(Qt::CheckStateRole), static_cast<role_t>(Qt::CheckStateRole));

  // usable in a constant expression
  static_assert(GetQtRole(Qt::DisplayRole) == static_cast<role_t>(Qt::DisplayRole));
}

}  // namespace mvvm::test
