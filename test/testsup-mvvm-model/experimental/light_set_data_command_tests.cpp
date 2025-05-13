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

#include "mvvm/experimental/light_set_data_command.h"

#include <mvvm/experimental/light_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm
{

class LightSetDataCommandTest : public ::testing::Test
{
public:
};

TEST_F(LightSetDataCommandTest, SetData)
{
  experimental::LightItem item;

  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(item.SetData(value));
  EXPECT_EQ(item.Data(), value);

  const variant_t new_value{mvvm::int32{43}};
  experimental::LightSetDataCommand command(&item, new_value, DataRole::kData);
  EXPECT_EQ(item.Data(), value);

  command.Execute();
  EXPECT_EQ(item.Data(), new_value);

  command.Undo();
  EXPECT_EQ(item.Data(), value);
}

}  // namespace mvvm
