/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/item_limits_helper.h"
#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Unit tests for helper functions defined in item_limits_helper.h

class ItemLimitsHelperTests : public ::testing::Test
{
};

//! Limits related method applied to item without limits set.

TEST_F(ItemLimitsHelperTests, DefaultConstructedItem)
{
  SessionItem item;
  EXPECT_FALSE(HasLowerLimit(item));
  EXPECT_EQ(GetLowerLimit(item), variant_t{});

  EXPECT_FALSE(HasUpperLimit(item));
  EXPECT_EQ(GetUpperLimit(item), variant_t{});

  // Item doesn't have own data, and limits defined. Any data will be "in range".
  EXPECT_TRUE(IsInRange(item, 42));

  EXPECT_TRUE(IsLimitless(item));

  EXPECT_FALSE(IsLimited(item));
}

TEST_F(ItemLimitsHelperTests, RemoveLimits)
{
  {  // remove two limits, when two old limits are present
    SessionItem item;

    item.SetData(42, DataRole::kLowerLimit);
    item.SetData(43, DataRole::kUpperLimit);
    EXPECT_TRUE(item.HasData(DataRole::kLowerLimit));
    EXPECT_TRUE(item.HasData(DataRole::kUpperLimit));

    RemoveLimits(item);
    EXPECT_FALSE(item.HasData(DataRole::kLowerLimit));
    EXPECT_FALSE(item.HasData(DataRole::kUpperLimit));
  }

  {  // remove two limits, when one limit is present (real life bug)
    SessionItem item;

    item.SetData(42, DataRole::kLowerLimit);
    EXPECT_TRUE(item.HasData(DataRole::kLowerLimit));

    RemoveLimits(item);
    EXPECT_FALSE(item.HasData(DataRole::kLowerLimit));
    EXPECT_FALSE(item.HasData(DataRole::kUpperLimit));
  }
}

TEST_F(ItemLimitsHelperTests, LowerLimited)
{
  {  // item has int data, setting limit [40, inf[
    SessionItem item;
    item.SetData(42);
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(40, item);
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_TRUE(IsInRange(item, 42));
    EXPECT_FALSE(IsInRange(item, 39));

    EXPECT_EQ(GetLowerLimit(item), variant_t(40));
  }

  {  // item do not have initial data, setting limit [40, inf[
    SessionItem item;
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(40, item);
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_TRUE(IsInRange(item, 42));
    EXPECT_FALSE(IsInRange(item, 39));

    EXPECT_EQ(GetLowerLimit(item), variant_t(40));
  }
}
