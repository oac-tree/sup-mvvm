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

#include "mvvm/utils/limited_integer.h"

#include <mvvm/core/exceptions.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Testing LimitedInteger class.

class LimitedIntegerTests : public ::testing::Test
{
};

TEST_F(LimitedIntegerTests, Constructor)
{
  {  // number without bounds defined
    const LimitedInteger<int> num(42, {}, {});
    EXPECT_EQ(num.GetValue(), 42);
    EXPECT_EQ(num.GetLowerBound(), std::numeric_limits<int>::min());
    EXPECT_EQ(num.GetUpperBound(), std::numeric_limits<int>::max());
  }

  {  // number with bounds
    const LimitedInteger<int> num(42, 1, 45);
    EXPECT_EQ(num.GetValue(), 42);
    EXPECT_EQ(num.GetLowerBound(), 1);
    EXPECT_EQ(num.GetUpperBound(), 45);
  }

  {  // attempt to mix types
    EXPECT_THROW(LimitedInteger<int>(42.1, {}, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>(42, 42.1, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>(42, 10, 64U), RuntimeException);
  }
}

TEST_F(LimitedIntegerTests, SetValue)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_TRUE(num.SetValue(42));
    EXPECT_TRUE(num.SetValue(43));
    EXPECT_EQ(num.GetValue(), 43);
  }

  {  // number with  upper limit
    LimitedInteger<int> num(42, {}, {50});
    EXPECT_TRUE(num.SetValue(49));
    EXPECT_TRUE(num.SetValue(50));
    EXPECT_FALSE(num.SetValue(51));
    EXPECT_EQ(num.GetValue(), 50);
  }

  {  // number with lower limit
    LimitedInteger<int> num(42, {40}, {});
    EXPECT_TRUE(num.SetValue(41));
    EXPECT_TRUE(num.SetValue(40));
    EXPECT_FALSE(num.SetValue(39));
    EXPECT_EQ(num.GetValue(), 40);
  }

  {  // number with upper and lower limits
    LimitedInteger<int> num(42, {40}, {50});
    EXPECT_TRUE(num.SetValue(40));
    EXPECT_TRUE(num.SetValue(50));
    EXPECT_FALSE(num.SetValue(39));
    EXPECT_FALSE(num.SetValue(51));
    EXPECT_EQ(num.GetValue(), 50);
  }
}
