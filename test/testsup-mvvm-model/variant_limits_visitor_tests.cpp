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

#include "mvvm/utils/variant_limits_visitor.h"

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests for VariantLimitsVisitor class.
 */
class VariantLimitsVisitorTest : public ::testing::Test
{
};

TEST_F(VariantLimitsVisitorTest, GetInt32MinMaxNumeric)
{
  {
    VariantLimitsVisitor<int> visitor;
    auto limits = GetMinMaxLimitsPair<int, mvvm::int8>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::int8{}), limits);
  }

  {
    VariantLimitsVisitor<int> visitor;
    auto limits = GetMinMaxLimitsPair<int, mvvm::uint8>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::uint8{}), limits);
  }

  {
    VariantLimitsVisitor<int> visitor;
    auto limits = GetMinMaxLimitsPair<int, mvvm::int16>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::int16{}), limits);
  }

  {
    VariantLimitsVisitor<int> visitor;
    auto limits = GetMinMaxLimitsPair<int, mvvm::uint16>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::uint16{}), limits);
  }

  {
    VariantLimitsVisitor<int> visitor;
    auto limits = GetMinMaxLimitsPair<int, mvvm::int32>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::int32{}), limits);
  }

  {
    VariantLimitsVisitor<float64> visitor;  // stores 32 limits
    auto limits = GetMinMaxLimitsPair<float64, mvvm::float32>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::float32{}), limits);
  }

  {
    VariantLimitsVisitor<float64> visitor;
    auto limits = GetMinMaxLimitsPair<float64, mvvm::float64>({}, {});
    EXPECT_EQ(visitor.operator()(mvvm::float64{}), limits);
  }

  {
    VariantLimitsVisitor<int> visitor;

    EXPECT_THROW(visitor.operator()(std::monostate{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(mvvm::boolean{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(mvvm::char8{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(mvvm::uint32{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(mvvm::int64{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(mvvm::uint64{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(std::string{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(std::vector<double>{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(ComboProperty{}), RuntimeException);
    EXPECT_THROW(visitor.operator()(ExternalProperty{}), RuntimeException);
  }

  // more tests in variant_limits_helper.h
}
