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

#include "mvvm/core/mvvm_errors.h"

#include <mvvm/core/mvvm_error_condition.h>

#include <gtest/gtest.h>

#include <set>

namespace mvvm::test
{

class MvvmErrorsTest : public ::testing::Test
{
public:
  /**
   * @brief Helper function to test error codes.
   */
  static std::error_code GetModelError(const std::string& error_name)
  {
    if (error_name == "NullItem")
    {
      return make_error_code(ModelError::NullItem);
    }
    if (error_name == "ItemHasNoParent")
    {
      return make_error_code(ModelError::ItemHasNoParent);
    }
    if (error_name == "ItemHasParent")
    {
      return make_error_code(ModelError::ItemHasParent);
    }

    return make_error_code(ModelError::Success);
  }
};

TEST_F(MvvmErrorsTest, BasicErrorCodeUsage)
{
  {  // NullItem error code
    const std::error_code error_code = GetModelError("NullItem");
    EXPECT_TRUE(error_code == ModelError::NullItem);
    EXPECT_EQ(error_code.value(), static_cast<int>(ModelError::NullItem));
    EXPECT_EQ(error_code.category().name(), std::string(kModelErrorCategory));
    EXPECT_EQ(error_code.message(), "null item");
    EXPECT_TRUE(error_code);
    EXPECT_TRUE(error_code != ModelError::Success);
    EXPECT_FALSE(error_code == ModelError::Success);
  }

  {  // Success error code
    const std::error_code error_code = GetModelError("Success");
    EXPECT_EQ(error_code.value(), static_cast<int>(ModelError::Success));
    EXPECT_EQ(error_code.category().name(), std::string(kModelErrorCategory));
    EXPECT_EQ(error_code.message(), "success");
    EXPECT_FALSE(error_code);
    EXPECT_TRUE(error_code == ModelError::Success);
    EXPECT_FALSE(error_code != ModelError::Success);
  }

  {  // unknown error
    const std::error_code error_code = make_error_code(static_cast<ModelError>(255));
    EXPECT_EQ(error_code.value(), 255);
    EXPECT_EQ(error_code.category().name(), std::string(kModelErrorCategory));
    EXPECT_EQ(error_code.message(), kUnknownErrorMessage);
    EXPECT_TRUE(error_code);
    EXPECT_TRUE(error_code != ModelError::Success);
    EXPECT_FALSE(error_code == ModelError::Success);
  }
}

TEST_F(MvvmErrorsTest, UniqueErrorMessages)
{
  // all messages are non-empty
  std::set<std::string> all_messages;
  for (std::int32_t index = 0; index < static_cast<std::int32_t>(ModelError::MaxCount); ++index)
  {
    const auto& error_code = make_error_code(static_cast<ModelError>(index));
    const std::string message = error_code.message();
    if (message != kUnknownErrorMessage && !message.empty())
    {
      all_messages.insert(message);
    }
  }

  // all error messages are different
  EXPECT_EQ(all_messages.size(), static_cast<std::size_t>(ModelError::MaxCount));
}

TEST_F(MvvmErrorsTest, UsingErrorCodesWithMvvmErrorCondition)
{
  const auto error_code = make_error_code(ModelError::NullItem);
  const auto error_condition = error_code.default_error_condition();
  EXPECT_EQ(error_condition.value(), static_cast<int>(MvvmErrorCondition::ModelError));
  EXPECT_EQ(error_condition.category().name(), std::string(kModelErrorCondition));
  EXPECT_EQ(error_condition.message(), "model error");

  EXPECT_TRUE(error_code == error_condition);

  const auto error_condition2 = make_error_condition(MvvmErrorCondition::ModelError);
  EXPECT_TRUE(error_condition == error_condition2);
  EXPECT_TRUE(error_code == error_condition2);

  EXPECT_TRUE(error_code == MvvmErrorCondition::ModelError);
  EXPECT_TRUE(make_error_code(ModelError::NullItem) == MvvmErrorCondition::ModelError);
}

TEST_F(MvvmErrorsTest, CheckAllErrorCodeForSameErrorCondition)
{
  const std::error_condition expected_error_condition = MvvmErrorCondition::ModelError;

  for (std::int32_t index = 0; index < static_cast<std::int32_t>(ModelError::MaxCount); ++index)
  {
    const auto& error_code = make_error_code(static_cast<ModelError>(index));
    const auto& error_condition = error_code.default_error_condition();
    EXPECT_EQ(error_condition, expected_error_condition);
    EXPECT_TRUE(error_code == expected_error_condition);
  }
}

}  // namespace mvvm::test
