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

  {  // attempt to ser upper limit smaller than lower limit
    EXPECT_THROW(LimitedInteger<int>(42, 10, 5), RuntimeException);
  }
}

TEST_F(LimitedIntegerTests, ConstructorWithValueShift)
{
  {  // value forced to upper limit
    const LimitedInteger<int> num(42, 10, 20);
    EXPECT_EQ(num.GetValue(), 20);
    EXPECT_EQ(num.GetLowerBound(), 10);
    EXPECT_EQ(num.GetUpperBound(), 20);
  }

  {  // value forced to lower limit
    const LimitedInteger<int> num(1, 10, 20);
    EXPECT_EQ(num.GetValue(), 10);
    EXPECT_EQ(num.GetLowerBound(), 10);
    EXPECT_EQ(num.GetUpperBound(), 20);
  }
}

TEST_F(LimitedIntegerTests, SetValue)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.SetValue(42));  // same value
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

TEST_F(LimitedIntegerTests, SetValueFromVariant)
{
  LimitedInteger<int> num(42, {}, {});
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{42}));  // same value
  EXPECT_TRUE(num.SetValueFromVariant(variant_t{43}));
  EXPECT_EQ(num.GetValue(), 43);

  EXPECT_EQ(num.GetValueAsVariant(), variant_t{43});
}

TEST_F(LimitedIntegerTests, Increment)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
    EXPECT_TRUE(num.Increment());
    EXPECT_EQ(num.GetValue(), 43);
  }

  {  // number with limits
    LimitedInteger<int> num(49, 40, 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Increment());

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());

    EXPECT_FALSE(num.Increment());

    EXPECT_EQ(num.GetValue(), 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());
  }

  {  // uint8 without limits
    uint8 initial_value(254);
    LimitedInteger<uint8> num(initial_value, {}, {});

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Increment());
    EXPECT_FALSE(num.Increment());

    EXPECT_EQ(num.GetValue(), 255);

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());
  }
}

TEST_F(LimitedIntegerTests, Decrement)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Decrement());

    EXPECT_EQ(num.GetValue(), 41);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // number with limits
    LimitedInteger<int> num(41, 40, 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Decrement());
    EXPECT_TRUE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_FALSE(num.Decrement());

    EXPECT_EQ(num.GetValue(), 40);
    EXPECT_TRUE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // uint8 without limits
    uint8 initial_value(1);
    LimitedInteger<uint8> num(initial_value, {}, {});
    EXPECT_TRUE(num.Decrement());
    EXPECT_FALSE(num.Decrement());
    EXPECT_EQ(num.GetValue(), 0);
  }
}

TEST_F(LimitedIntegerTests, SetValueFromText)
{
  LimitedInteger<int> num(42, {}, {});

  EXPECT_TRUE(num.SetValueFromText("43"));
  EXPECT_EQ(num.GetValue(), 43);
  EXPECT_EQ(num.GetValueAsText(), std::string("43"));

  EXPECT_FALSE(num.SetValueFromText("43"));  // // same value

  // Our current implementation allows such conversions. We rely on validators on board of cell
  // editors.
  EXPECT_TRUE(num.SetValueFromText("44.1"));
  EXPECT_EQ(num.GetValue(), 44);

  EXPECT_FALSE(num.SetValueFromText("aaaa"));
}

TEST_F(LimitedIntegerTests, StepBy)
{
  {  // positive ulimited integer
    LimitedInteger<int> num(42, {}, {});

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 43);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 42);
  }

  {  // negative ulimited integer
    LimitedInteger<int> num(-42, {}, {});

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), -41);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), -42);
  }

  {  // limited integer, several incrementing steps
    LimitedInteger<int> num(42, 40, 50);

    EXPECT_FALSE(num.StepBy(0));
    EXPECT_EQ(num.GetValue(), 42);

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 43);

    EXPECT_TRUE(num.StepBy(2));
    EXPECT_EQ(num.GetValue(), 45);

    EXPECT_TRUE(num.StepBy(5));
    EXPECT_EQ(num.GetValue(), 50);

    EXPECT_FALSE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 50);
  }

  {  // limited integer, several decrementing steps
    LimitedInteger<int> num(45, 40, 50);

    EXPECT_FALSE(num.StepBy(0));
    EXPECT_EQ(num.GetValue(), 45);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 44);

    EXPECT_TRUE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), 42);

    EXPECT_TRUE(num.StepBy(-5));
    EXPECT_EQ(num.GetValue(), 40);  // stopped at minimum border

    EXPECT_FALSE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 40);
  }

  {  // unlimited int8, several steps
    int8 initial_value{0};
    LimitedInteger<int8> num(initial_value, {}, {});

    EXPECT_TRUE(num.StepBy(300));
    EXPECT_EQ(num.GetValue(), 127);
    EXPECT_FALSE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 127);

    EXPECT_TRUE(num.StepBy(-300));
    EXPECT_EQ(num.GetValue(), -128);
    EXPECT_FALSE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), -128);
  }

  {  // unlimited uint8, several steps
    uint8 initial_value{0};
    LimitedInteger<uint8> num(initial_value, {}, {});

    EXPECT_TRUE(num.StepBy(2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(2));
    EXPECT_TRUE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(0));
    EXPECT_FALSE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(0));
  }
}
