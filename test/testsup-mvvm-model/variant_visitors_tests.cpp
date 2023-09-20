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

#include "mvvm/core/variant_visitors.h"

#include <gtest/gtest.h>

#include <limits>

using namespace mvvm;

//! Unit tests for visitors defined in variant_visitors.h.

class VariantVisitorsTests : public ::testing::Test
{
};

TEST_F(VariantVisitorsTests, GetInt32MinMaxNumeric)
{
  {  // int32
    int32 num(42);
    auto limits = GetInt32MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<int32>::min());
    EXPECT_EQ(limits.second, std::numeric_limits<int32>::max());
  }

  {  // uint8
    uint8 num(1);
    auto limits = GetInt32MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<uint8>::min());
    EXPECT_EQ(limits.second, std::numeric_limits<uint8>::max());
    EXPECT_EQ(limits.first, 0);
    EXPECT_EQ(limits.second, 255);
  }

  // uint32, double limits can't be handled by given helper function
  EXPECT_THROW(GetInt32MinMaxNumeric(42U), RuntimeException);
  EXPECT_THROW(GetInt32MinMaxNumeric(42.2), RuntimeException);
}
