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

#include "mvvm/utils/limited_integer_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/limited_integer.h>

#include <gtest/gtest.h>

#include <memory>

namespace mvvm::test
{

/**
 * @brief Testing helper methods from limited_integer_helper.h
 */
class LimitedIntegerHelperTests : public ::testing::Test
{
public:
  /**
   * @brief Helper function that tries to create limited integer for given type and then check if
   * correct type was created.
   */
  template <typename IntegerT>
  bool CanCreateLimitedInteger()
  {
    IntegerT inital_value{0};
    variant_t data(inital_value);

    auto limited_integer = CreateLimitedInteger(data, {}, {});
    return dynamic_cast<LimitedInteger<IntegerT>*>(limited_integer.get());
  }
};

TEST_F(LimitedIntegerHelperTests, CreateLimitedInteger)
{
  //  EXPECT_TRUE(CanCreateLimitedInteger<char8>()); // temporarily disabled due to int8/char8
  //  ambiguity and MISRA complains
  EXPECT_TRUE(CanCreateLimitedInteger<int8>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint8>());
  EXPECT_TRUE(CanCreateLimitedInteger<int16>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint16>());
  EXPECT_TRUE(CanCreateLimitedInteger<int32>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint32>());
  EXPECT_TRUE(CanCreateLimitedInteger<int64>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint64>());

  EXPECT_THROW(CanCreateLimitedInteger<float64>(), RuntimeException);

  EXPECT_FALSE(CreateLimitedInteger(variant_t{42}, {}, {})->IsNullable());
}

TEST_F(LimitedIntegerHelperTests, CreateLimitedIntegerWithBounds)
{
  auto num = CreateLimitedInteger(variant_t{42}, variant_t{10}, variant_t{50});
  EXPECT_EQ(num->GetLowerBoundAsVariant(), variant_t{10});
  EXPECT_EQ(num->GetUpperBoundAsVariant(), variant_t{50});

  // bounds of another type
  EXPECT_THROW(CreateLimitedInteger(variant_t{42}, variant_t{42.1}, {}), RuntimeException);
  EXPECT_THROW(CreateLimitedInteger(variant_t{42}, variant_t{10}, variant_t{64U}),
               RuntimeException);

  // upper bound smaller than lower bound
  EXPECT_THROW(CreateLimitedInteger(variant_t{42}, variant_t{10}, variant_t{5}), RuntimeException);
}

TEST_F(LimitedIntegerHelperTests, CreateNullableLimitedInteger)
{
  {  // without value
    auto num = CreateNullableLimitedInteger({}, variant_t{uint32{42}}, {}, {});
    EXPECT_NE(dynamic_cast<LimitedInteger<uint32>*>(num.get()), nullptr);
    EXPECT_TRUE(num->IsNullable());
    EXPECT_FALSE(num->HasValue());
  }

  {  // with value
    auto num = CreateNullableLimitedInteger(variant_t{uint32{1}}, variant_t{uint32{42}}, {}, {});
    EXPECT_EQ(num->GetValueAsVariant(), variant_t{uint32{1}});
    EXPECT_TRUE(num->IsNullable());
    EXPECT_TRUE(num->HasValue());
  }

  {  // type is defined by default value
    EXPECT_THROW(CreateNullableLimitedInteger({}, {}, {}, {}), RuntimeException);
    EXPECT_THROW(CreateNullableLimitedInteger({}, variant_t{42.0}, {}, {}), RuntimeException);
    EXPECT_THROW(CreateNullableLimitedInteger(variant_t{int32{1}}, variant_t{uint32{42}}, {}, {}),
                 RuntimeException);
    EXPECT_THROW(CreateNullableLimitedInteger({}, variant_t{uint32{42}}, variant_t{10}, {}),
                 RuntimeException);
  }
}

}  // namespace mvvm::test
