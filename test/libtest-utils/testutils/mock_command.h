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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_

#include "mvvm/commands/abstract_command.h"

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockCommand class to test AbstractCommand logic.
 */
class MockCommand : public AbstractCommand
{
public:
  MOCK_METHOD(void, ExecuteImpl, ());
  MOCK_METHOD(void, UndoImpl, ());
};

/**
 * @brief The TestCommand class is intended to test the order of execution of ExecuteImpl and
 * UndoImpl, when several commands are placed in a stack.
 */
class TestCommand : public AbstractCommand
{
public:
  using callback_t = std::function<void(ICommand*)>;

  TestCommand() = default;

  explicit TestCommand(callback_t execute_callback, callback_t undo_callback)
      : m_execute_callback(std::move(execute_callback)), m_undo_callback(std::move(undo_callback))
  {
  }

  void SetMakeObsoleteAfterExecution() { m_make_obsolete = true; }

private:
  void ExecuteImpl() override
  {
    if (m_execute_callback)
    {
      m_execute_callback(this);
    }

    if (m_make_obsolete)
    {
      SetIsObsolete(true);
    }
  }

  void UndoImpl() override
  {
    if (m_undo_callback)
    {
      m_undo_callback(this);
    }
  }

  callback_t m_execute_callback;
  callback_t m_undo_callback;
  bool m_make_obsolete{false};
};

/**
 * @brief The MockTestCommandListener class is to solely listen to TestCommand.
 */
class MockTestCommandListener
{
public:
  MOCK_METHOD(void, OnExecute, (ICommand*));
  MOCK_METHOD(void, OnUndo, (ICommand*));
};

/**
 * @brief Creates test command.
 *
 * @param listener The external listener to send command's reports.
 * @return Unique_ptr to the command, and convenience raw pointer.
 */
std::pair<std::unique_ptr<TestCommand>, ICommand*> CreateCommand(MockTestCommandListener& listener);

/**
 * @brief Creates test command.
 */
std::pair<std::unique_ptr<TestCommand>, ICommand*> CreateCommand();

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_
