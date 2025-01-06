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

#ifndef MVVM_COMMANDS_MACRO_COMMAND_H_
#define MVVM_COMMANDS_MACRO_COMMAND_H_

#include <mvvm/commands/abstract_command.h>

#include <vector>

namespace mvvm
{

/**
 * @brief The MacroCommand class contains a chain of children commands, which will be executed one
 * after another, on a single Execute call.
 *
 * Similarly, it will undo all its children on a single Undo call.
 */
class MVVM_MODEL_EXPORT MacroCommand : public AbstractCommand
{
public:
  explicit MacroCommand(const std::string& description);

  MacroCommand(const MacroCommand& other) = delete;
  MacroCommand& operator=(const MacroCommand& other) = delete;

  CommandStatus GetCommandStatus() const override;

  /**
   * @brief Appends children command to the list of children.
   */
  void Append(std::unique_ptr<ICommand> command);

  /**
   * @brief Returns number of commands in given macro.
   */
  size_t GetCommandCount() const;

  /**
   * @brief Returns list of all commands.
   */
  std::vector<const ICommand *> GetCommands() const;

private:
  void ExecuteImpl() override;

  void UndoImpl() override;

  std::vector<std::unique_ptr<ICommand>> m_children;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_MACRO_COMMAND_H_
