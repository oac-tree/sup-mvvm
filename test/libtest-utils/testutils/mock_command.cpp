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

#include "mock_command.h"

namespace mvvm::test
{

std::pair<std::unique_ptr<TestCommand>, ICommand *> CreateCommand(MockTestCommandListener &listener)
{
  auto on_execute = [&listener](auto ptr) { listener.OnExecute(ptr); };
  auto on_undo = [&listener](auto ptr) { listener.OnUndo(ptr); };
  auto command = std::make_unique<TestCommand>(on_execute, on_undo);
  auto command_ptr = command.get();
  return {std::move(command), command_ptr};
}

std::pair<std::unique_ptr<TestCommand>, ICommand *> CreateCommand()
{
  auto command = std::make_unique<TestCommand>();
  auto command_ptr = command.get();
  return {std::move(command), command_ptr};
}

}  // namespace mvvm::test
