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

#include "mvvm/core/mvvm_exceptions.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ExceptionsTests : public ::testing::Test
{
};

TEST_F(ExceptionsTests, Message)
{
  const std::string message("custom message");

  {
    auto exception = RuntimeException(message);
    EXPECT_EQ(std::string(exception.what()), message);
  }

  {
    auto exception = NullArgumentException(message);
    EXPECT_EQ(std::string(exception.what()), message);
  }

  {
    auto exception = NotImplementedException(message);
    EXPECT_EQ(std::string(exception.what()), message);
  }

  {
    auto exception = InvalidOperationException(message);
    EXPECT_EQ(std::string(exception.what()), message);
  }

  {
    auto exception = LogicErrorException(message);
    EXPECT_EQ(std::string(exception.what()), message);
  }
}
