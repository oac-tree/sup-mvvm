/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/exceptions.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ExceptionsTest : public ::testing::Test
{
};

TEST_F(ExceptionsTest, Message)
{
  std::string message("custom message");

  auto exception1 = RuntimeException(message);
  EXPECT_EQ(std::string(exception1.what()), message);

  auto exception2 = InvalidMoveException(message);
  EXPECT_EQ(std::string(exception2.what()), message);
}
