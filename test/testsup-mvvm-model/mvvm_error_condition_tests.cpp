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

#include "mvvm/core/mvvm_error_condition.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

class MvvmErrorConditionTest : public ::testing::Test
{
};

TEST_F(MvvmErrorConditionTest, BasicErrorConditionUsage)
{
  auto condition = make_error_condition(MvvmErrorCondition::ModelError);
  EXPECT_EQ(condition.message(), "model error");
  EXPECT_EQ(condition.value(), static_cast<int>(MvvmErrorCondition::ModelError));
}

}  // namespace mvvm::test
