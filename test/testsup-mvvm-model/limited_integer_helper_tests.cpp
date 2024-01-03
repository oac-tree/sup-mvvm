/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/limited_integer_helper.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/limited_integer.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Testing helper methods from limited_integer_helper.h

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
  EXPECT_TRUE(CanCreateLimitedInteger<int8>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint8>());
  EXPECT_TRUE(CanCreateLimitedInteger<int16>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint16>());
  EXPECT_TRUE(CanCreateLimitedInteger<int32>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint32>());
  EXPECT_TRUE(CanCreateLimitedInteger<int64>());
  EXPECT_TRUE(CanCreateLimitedInteger<uint64>());

  EXPECT_THROW(CanCreateLimitedInteger<float64>(), RuntimeException);
}
