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

#include "mvvm/experimental/light_item.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm
{

class LightItemTest : public ::testing::Test
{
public:
};

TEST_F(LightItemTest, SetData)
{
  experimental::LightItem item;

  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(item.SetData(value));
  EXPECT_EQ(item.Data(), value);
}

TEST_F(LightItemTest, SetDataViaStrategy)
{
  experimental::LightItem item;

  auto strategy = [](auto item, const variant_t& value, std::int32_t role) -> bool
  { return item->SetDataIntern(value, role); };
  item.SetDataStrategy(strategy);

  const variant_t value = mvvm::int32(42);
  EXPECT_TRUE(item.SetData(value));
  EXPECT_EQ(item.Data(), value);

  auto increment_strategy = [](auto item, const variant_t& value, std::int32_t role) -> bool
  {
    mvvm::int32 int_value = std::get<mvvm::int32>(value);
    return item->SetDataIntern(++int_value, role);
  };
  item.SetDataStrategy(increment_strategy);

  const variant_t new_value{mvvm::int32{43}};
  const variant_t expected_value{mvvm::int32{44}};
  EXPECT_TRUE(item.SetData(new_value));
  EXPECT_EQ(item.Data(), expected_value);
}

}  // namespace mvvm
