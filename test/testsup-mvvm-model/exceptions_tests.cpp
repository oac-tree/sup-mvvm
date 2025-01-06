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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/exceptions.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ExceptionsTests : public ::testing::Test
{
};

TEST_F(ExceptionsTests, Message)
{
  std::string message("custom message");

  auto exception1 = RuntimeException(message);
  EXPECT_EQ(std::string(exception1.what()), message);

  auto exception2 = InvalidOperationException(message);
  EXPECT_EQ(std::string(exception2.what()), message);

  auto exception3 = KeyNotFoundException(message);
  EXPECT_EQ(std::string(exception3.what()), message);

  auto exception4 = ExistingKeyException(message);
  EXPECT_EQ(std::string(exception4.what()), message);
}
