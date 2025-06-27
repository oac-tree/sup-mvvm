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

#include "mvvm/model/process_command.h"

#include <mvvm/commands/set_value_command.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class ProcessCommandTest : public ::testing::Test
{
};

TEST_F(ProcessCommandTest, ProcessCommand)
{
  SessionModel model;
  auto item = model.InsertItem<SessionItem>();

  auto result = ProcessCommand<SetValueCommand>(model, item, int32{42}, DataRole::kData);
  EXPECT_TRUE(result);
  EXPECT_EQ(item->Data<int32>(), mvvm::int32{42});
}

}  // namespace mvvm::test
