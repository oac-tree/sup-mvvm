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

#include "mock_command.h"

namespace mvvm::test
{

std::pair<std::unique_ptr<TestCommand>, ICommand *> CreateCommand(MockTestCommandListener &listener)
{
  auto command = std::make_unique<TestCommand>(listener.CreateContext());
  auto command_ptr = command.get();
  return {std::move(command), command_ptr};
}

std::pair<std::unique_ptr<TestCommand>, ICommand *> CreateCommand()
{
  auto command = std::make_unique<TestCommand>();
  auto command_ptr = command.get();
  return {std::move(command), command_ptr};
}

TestCommandContext MockTestCommandListener::CreateContext()
{
  TestCommandContext result;

  result.execute_impl = [this](auto ptr) { OnExecute(ptr); };
  result.undo_impl = [this](auto ptr) { OnUndo(ptr); };
  result.event_before = [this](auto ptr) { return OnGetEventBefore(ptr); };
  result.event_after = [this](auto ptr) { return OnGetEventAfter(ptr); };
  result.notify = [this](auto ptr, const event_variant_t &event)
  { return OnNotify(ptr, event); };
  return result;
}

}  // namespace mvvm::test
