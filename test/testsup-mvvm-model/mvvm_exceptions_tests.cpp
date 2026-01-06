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

#include "mvvm/core/mvvm_exceptions.h"

#include <mvvm/core/mvvm_errors.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class MvvmExceptionsTest : public ::testing::Test
{
};

TEST_F(MvvmExceptionsTest, Message)
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

  {
    const std::error_code error_code = make_error_code(ModelError::NullItem);
    auto exception = SystemException(error_code, message);
    // Looks liks std::exception concatenates messages so it becomes "custom message: null item"
    EXPECT_EQ(std::string(exception.what()), message + ": " + error_code.message());
    EXPECT_EQ(exception.code(), error_code);
  }

  {
    const std::error_code error_code = ModelError::NullItem;
    auto exception = SystemException(error_code);
    EXPECT_EQ(std::string(exception.what()), error_code.message());
    EXPECT_EQ(exception.code(), error_code);
  }
}

}  // namespace mvvm::test
