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
  MOCK_METHOD(void, ExecuteImpl, (), (override));
  MOCK_METHOD(void, UndoImpl, (), (override));
  MOCK_METHOD(std::optional<event_variant_t>, GetEventBefore, (), (const, override));
  MOCK_METHOD(std::optional<event_variant_t>, GetEventAfter, (), (const, override));
  MOCK_METHOD(void, Notify, (const event_variant_t&), (override));
};

/**
 * @brief Collection of callbacks for TestCommand reports.
 */
struct TestCommandContext
{
  std::function<void(const ICommand*)> execute_impl;
  std::function<void(const ICommand*)> undo_impl;
  std::function<std::optional<event_variant_t>(const ICommand*)> event_before;
  std::function<std::optional<event_variant_t>(const ICommand*)> event_after;
  std::function<void(const ICommand*, const event_variant_t& event)> notify;
};

/**
 * @brief The TestCommand class is intended to test the order of execution of ExecuteImpl and
 * UndoImpl, when several commands are placed in a stack.
 *
 * It reports its activity via TestCommandContext.
 */
class TestCommand : public AbstractCommand
{
public:
  using callback_t = std::function<void(ICommand*)>;

  TestCommand() = default;

  explicit TestCommand(const TestCommandContext& context) : m_context(context) {}

  void SetMakeObsoleteAfterExecution() { m_make_obsolete = true; }

private:
  void ExecuteImpl() override
  {
    if (m_context.execute_impl)
    {
      m_context.execute_impl(this);
    }

    if (m_make_obsolete)
    {
      SetIsObsolete(true);
    }
  }

  void UndoImpl() override
  {
    if (m_context.undo_impl)
    {
      m_context.undo_impl(this);
    }
  }

  std::optional<event_variant_t> GetEventBefore() const override
  {
    if (m_context.event_before)
    {
      return m_context.event_before(this);
    }

    return {};
  }

  std::optional<event_variant_t> GetEventAfter() const override
  {
    if (m_context.event_after)
    {
      return m_context.event_after(this);
    }

    return {};
  }

  void Notify(const event_variant_t& event) override
  {
    if (m_context.notify)
    {
      m_context.notify(this, event);
    }
  }

  TestCommandContext m_context;
  bool m_make_obsolete{false};
};

/**
 * @brief The MockTestCommandListener class is to solely listen to TestCommand.
 *
 * It can listen multiple commands and reports their addresses together with called methods.
 */
class MockTestCommandListener
{
public:
  MOCK_METHOD(void, OnExecute, (const ICommand*));
  MOCK_METHOD(void, OnUndo, (const ICommand*));
  MOCK_METHOD(std::optional<event_variant_t>, OnGetEventBefore, (const ICommand*), (const));
  MOCK_METHOD(std::optional<event_variant_t>, OnGetEventAfter, (const ICommand*), (const));
  MOCK_METHOD(void, OnNotify, (const ICommand*, const event_variant_t& event), (const));

  TestCommandContext CreateContext();
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
