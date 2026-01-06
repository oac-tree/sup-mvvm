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

#include "mvvm/commands/model_command.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/session_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

namespace mvvm::test
{

/**
 * @brief Tests for ModelCommand class.
 */
class ModelCommandTest : public ::testing::Test
{
public:
  /**
   * @brief Test implementation of ModelCommand.
   */
  class TestCommand : public ModelCommand
  {
  public:
    explicit TestCommand(ISessionModel* model) : ModelCommand(model) {}

  private:
    void ExecuteImpl() override {}
    void UndoImpl() override {}
  };
};

TEST_F(ModelCommandTest, InitialState)
{
  EXPECT_THROW(TestCommand(nullptr), RuntimeException);

  SessionModel session_model;
  EXPECT_NO_THROW((TestCommand(&session_model)));

  ApplicationModel application_model;
  EXPECT_NO_THROW((TestCommand(&application_model)));
}

}  // namespace mvvm::test
