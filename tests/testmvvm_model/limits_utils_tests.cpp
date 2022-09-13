/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/limits_utils.h"

#include <gtest/gtest.h>
#include <mvvm/model/limits.h>

using namespace mvvm;

//! Tests for Limits class.

class LimitsUtilsTests : public ::testing::Test
{
};

TEST_F(LimitsUtilsTests, IntLimitsToString)
{
  EXPECT_EQ(utils::ToString(IntLimits::CreateLimitless()), "limitless");
  EXPECT_EQ(utils::ToString(IntLimits::CreatePositive()), "positive");
  EXPECT_EQ(utils::ToString(IntLimits::CreateNonnegative()), "nonnegative");
  EXPECT_EQ(utils::ToString(IntLimits::CreateLowerLimited(42)), "lowerlimited;42");
  EXPECT_EQ(utils::ToString(IntLimits::CreateUpperLimited(1)), "upperlimited;1");
  EXPECT_EQ(utils::ToString(IntLimits::CreateLimited(-1, 2)), "limited;-1;2");
}
