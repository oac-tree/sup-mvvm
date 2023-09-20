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

#include <mvvm/core/exceptions.h>

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

//! Testing all methods related to SetLowerLimited.

TEST_F(ItemLimitsHelperTests, LowerLimited)
{
  {  // item has int data, setting limit [40, inf[
    SessionItem item;
    item.SetData(42);
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(40, item);

    EXPECT_EQ(GetLowerLimit(item), variant_t(40));
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(IsLimited(item));
    EXPECT_FALSE(IsLimitless(item));

    EXPECT_FALSE(IsInRange(item, 39));
    EXPECT_TRUE(IsInRange(item, 40));
    EXPECT_TRUE(IsInRange(item, 42));
  }

  {
    // Item do not have initial data, setting limit [40, inf[. The behavior is the same is before,
    // since it is allowed to have limits without the data
    SessionItem item;
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(40, item);

    EXPECT_EQ(GetLowerLimit(item), variant_t(40));
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(IsLimited(item));
    EXPECT_FALSE(IsLimitless(item));

    EXPECT_FALSE(IsInRange(item, 39));
    EXPECT_TRUE(IsInRange(item, 40));
    EXPECT_TRUE(IsInRange(item, 42));
  }

  {  // setting lower limit twice
    SessionItem item;
    item.SetData(42);
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(40, item);
    SetLowerLimited(41, item);

    EXPECT_EQ(GetLowerLimit(item), variant_t(41));
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_FALSE(IsLimited(item));
    EXPECT_FALSE(IsLimitless(item));

    EXPECT_FALSE(IsInRange(item, 39));
    EXPECT_FALSE(IsInRange(item, 40));
    EXPECT_TRUE(IsInRange(item, 42));
  }

  {  // attempt to set limits of wrong type
    SessionItem item;
    item.SetData(42.2);
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));

    SetLowerLimited(42.0, item);

    // setting integer limit instead of the double
    EXPECT_THROW(SetLowerLimited(40, item), LogicErrorException);

    // old limits should be preserved
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_FALSE(HasUpperLimit(item));
    EXPECT_EQ(GetLowerLimit(item), variant_t(42.0));
  }
}

//! Testing all methods related to SetUpperLimited.

TEST_F(ItemLimitsHelperTests, UpperLimited)
{
  {  // item has double data, setting limit [-inf, 5.0[
    SessionItem item;
    item.SetData(1.0);

    SetUpperLimited(5.0, item);

    EXPECT_EQ(GetLowerLimit(item), variant_t{});
    EXPECT_EQ(GetUpperLimit(item), variant_t(5.0));
    EXPECT_FALSE(HasLowerLimit(item));
    EXPECT_TRUE(HasUpperLimit(item));
    EXPECT_FALSE(IsLimited(item));
    EXPECT_FALSE(IsLimitless(item));

    EXPECT_TRUE(IsInRange(item, -std::numeric_limits<double>::infinity()));
    EXPECT_TRUE(IsInRange(item, -2.0));
    EXPECT_TRUE(IsInRange(item, 4.9));
    EXPECT_FALSE(IsInRange(item, 5.0));
    EXPECT_FALSE(IsInRange(item, std::numeric_limits<double>::infinity()));
  }
}

//! Testing all methods related to SetLimited.

TEST_F(ItemLimitsHelperTests, Limited)
{
  {  // item has double data, setting limit [-inf, 5.0[
    SessionItem item;
    item.SetData(1.0);

    SetLimited(-10.0, 2.0, item);

    EXPECT_EQ(GetLowerLimit(item), variant_t{-10.0});
    EXPECT_EQ(GetUpperLimit(item), variant_t(2.0));
    EXPECT_TRUE(HasLowerLimit(item));
    EXPECT_TRUE(HasUpperLimit(item));
    EXPECT_TRUE(IsLimited(item));
    EXPECT_FALSE(IsLimitless(item));

    EXPECT_FALSE(IsInRange(item, -11.0));
    EXPECT_TRUE(IsInRange(item, -9.0));
    EXPECT_TRUE(IsInRange(item, 0.0));
    EXPECT_TRUE(IsInRange(item, 1.0));
    EXPECT_FALSE(IsInRange(item, 2.0));
    EXPECT_FALSE(IsInRange(item, 3.0));
  }
}
