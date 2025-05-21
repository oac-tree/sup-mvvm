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
  MockCommandStack()
  {
    // will save command in a local cache to avoid its deletion
    ON_CALL(*this, Execute)
        .WillByDefault(
            [this](std::unique_ptr<ICommand> command)
            {
              m_last_command = std::move(command);
              return m_last_command.get();
            });
  }

  MOCK_METHOD(ICommand*, Execute, (std::unique_ptr<ICommand> command), (override));
  MOCK_METHOD(bool, CanUndo, (), (const, override));
  MOCK_METHOD(bool, CanRedo, (), (const, override));
  MOCK_METHOD(std::size_t, GetIndex, (), (const, override));
  MOCK_METHOD(std::size_t, GetCommandCount, (), (const, override));
  MOCK_METHOD(void, Undo, (), (override));
  MOCK_METHOD(void, Redo, (), (override));
  MOCK_METHOD(void, Clear, (), (override));
  MOCK_METHOD(void, SetUndoLimit, (std::size_t), (override));
  MOCK_METHOD(void, BeginMacro, (const std::string&), (override));
  MOCK_METHOD(void, EndMacro, (), (override));

  MOCK_METHOD(ICommand*, GetNextUndoCommand, (), (const));
  MOCK_METHOD(ICommand*, GetNextRedoCommand, (), (const));

  std::unique_ptr<ICommand> m_last_command;
};

/**
 * @brief The MockCommandStackDecorator is a decorator that forwards all calls to MockCommandStack.
 *
 * It is intended to be used as a template base class for NotifyingCommandStack validation.
 * It forward calls to MockCommandStack (we can't derive from MockCommandStack directly because
 * gmock is too complicated for that).
 */
class MockCommandStackDecorator : public ICommandStack
{
public:
  MockCommandStackDecorator(MockCommandStack* decoratee);

  ICommand* Execute(std::unique_ptr<ICommand> command) override;
  bool CanUndo() const override;
  bool CanRedo() const override;
  std::size_t GetIndex() const override;
  std::size_t GetCommandCount() const override;
  void Undo() override;
  void Redo() override;
  void Clear() override;
  void SetUndoLimit(std::size_t limit) override;
  void BeginMacro(const std::string& name) override;
  void EndMacro() override;

  const ICommand* GetNextUndoCommand() const;
  const ICommand* GetNextRedoCommand() const;

private:
  MockCommandStack* m_decoratee{nullptr};
};
}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_STACK_H_
