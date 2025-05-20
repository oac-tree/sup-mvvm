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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_STACK_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_STACK_H_

#include "mvvm/commands/i_command_stack.h"

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockCommandStack listens for ICommandStack interface calls.
 */
class MockCommandStack : public ICommandStack
{
public:
  MOCK_METHOD(ICommand*, Execute, (std::unique_ptr<ICommand> command), (override));
  MOCK_METHOD(bool, CanUndo, (), (const, override));
  MOCK_METHOD(bool, CanRedo, (), (const, override));
  MOCK_METHOD(int, GetIndex, (), (const, override));
  MOCK_METHOD(int, GetCommandCount, (), (const, override));
  MOCK_METHOD(std::vector<const ICommand*>, GetCommands, (), (const, override));
  MOCK_METHOD(void, Undo, (), (const));
  MOCK_METHOD(void, Redo, (), (const));
  MOCK_METHOD(void, Clear, (), (const));
  MOCK_METHOD(void, SetUndoLimit, (std::size_t), (const));
  MOCK_METHOD(void, BeginMacro, (std::string), ());
  MOCK_METHOD(void, EndMacro, (std::string), ());
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_STACK_H_
