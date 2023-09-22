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
    const LimitedInteger num(42, {}, {});
    EXPECT_EQ(num.GetValue(), variant_t(42));
    EXPECT_EQ(num.GetLowerBound(), variant_t());
    EXPECT_EQ(num.GetUpperBound(), variant_t());
  }

  {  // number with bounds
    const LimitedInteger num(42, 1, 45);
    EXPECT_EQ(num.GetValue(), variant_t(42));
    EXPECT_EQ(num.GetLowerBound(), variant_t(1));
    EXPECT_EQ(num.GetUpperBound(), variant_t(45));
  }

  {  // attempt to mix types
    EXPECT_THROW(LimitedInteger(42.1, {}, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger(42, 42.1, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger(42, 10, 64U), RuntimeException);
  }
}
