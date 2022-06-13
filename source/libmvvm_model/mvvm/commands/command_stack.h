/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_COMMANDS_COMMANDSTACK_H
#define MVVM_COMMANDS_COMMANDSTACK_H

#include "mvvm/commands/command_stack_interface.h"

#include <memory>

namespace mvvm
{

//! Base implementation of command stack.

class MVVM_MODEL_EXPORT CommandStack : public CommandStackInterface
{
public:
  void Execute(std::unique_ptr<CommandInterface> command) override;
  bool CanUndo() const override;
  bool CanRedo() const override;
  int GetIndex() const override;
  int GetSize() const override;
  void Undo() override;
  void Redo() override;
  void Clear() override;
  void SetUndoLimit(int limit) override;
  void BeginMacro(const std::string &name) override;
  void EndMacro() override;

private:


};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMANDSTACK_H
