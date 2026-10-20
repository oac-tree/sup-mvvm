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

#include "mvvm/utils/limited_float_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/limited_float.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Testing helper methods from limited_float_helper.h
 */
class LimitedFloatHelperTests : public ::testing::Test
{
};

TEST_F(LimitedFloatHelperTests, CreateLimitedFloat)
{
  auto num32 = CreateLimitedFloat(variant_t{42.0F}, {}, {});
  EXPECT_NE(dynamic_cast<LimitedFloat<float32>*>(num32.get()), nullptr);
  EXPECT_FALSE(num32->IsNullable());

  auto num64 = CreateLimitedFloat(variant_t{42.0}, {}, {});
  EXPECT_NE(dynamic_cast<LimitedFloat<float64>*>(num64.get()), nullptr);
  EXPECT_EQ(num64->GetValueAsVariant(), variant_t{42.0});

  EXPECT_THROW(CreateLimitedFloat(variant_t{42}, {}, {}), RuntimeException);
  EXPECT_THROW(CreateLimitedFloat({}, {}, {}), RuntimeException);
}

TEST_F(LimitedFloatHelperTests, CreateLimitedFloatWithBounds)
{
  auto num = CreateLimitedFloat(variant_t{42.0}, variant_t{10.0}, variant_t{50.0});
  EXPECT_EQ(num->GetLowerBoundAsVariant(), variant_t{10.0});
  EXPECT_EQ(num->GetUpperBoundAsVariant(), variant_t{50.0});

  // bounds of another type
  EXPECT_THROW(CreateLimitedFloat(variant_t{42.0}, variant_t{10}, {}), RuntimeException);
  EXPECT_THROW(CreateLimitedFloat(variant_t{42.0}, {}, variant_t{50.0F}), RuntimeException);

  // upper bound smaller than lower bound
  EXPECT_THROW(CreateLimitedFloat(variant_t{42.0}, variant_t{10.0}, variant_t{5.0}),
               RuntimeException);
}

TEST_F(LimitedFloatHelperTests, CreateNullableLimitedFloat)
{
  {  // without value
    auto num = CreateNullableLimitedFloat({}, variant_t{42.0F}, {}, {});
    EXPECT_NE(dynamic_cast<LimitedFloat<float32>*>(num.get()), nullptr);
    EXPECT_TRUE(num->IsNullable());
    EXPECT_FALSE(num->HasValue());
  }

  {  // with value
    auto num = CreateNullableLimitedFloat(variant_t{1.0}, variant_t{42.0}, {}, {});
    EXPECT_EQ(num->GetValueAsVariant(), variant_t{1.0});
    EXPECT_TRUE(num->IsNullable());
    EXPECT_TRUE(num->HasValue());
  }

  {  // type is defined by default value
    EXPECT_THROW(CreateNullableLimitedFloat({}, {}, {}, {}), RuntimeException);
    EXPECT_THROW(CreateNullableLimitedFloat({}, variant_t{42}, {}, {}), RuntimeException);
    EXPECT_THROW(CreateNullableLimitedFloat(variant_t{1.0F}, variant_t{42.0}, {}, {}),
                 RuntimeException);
    EXPECT_THROW(CreateNullableLimitedFloat({}, variant_t{42.0}, variant_t{10.0F}, {}),
                 RuntimeException);
  }
}

}  // namespace mvvm::test
