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

#ifndef MVVM_COMMANDS_ABSTRACT_COMMAND_H_
#define MVVM_COMMANDS_ABSTRACT_COMMAND_H_

#include <mvvm/commands/i_command.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The AbstractCommand class provides partial implementation of ICommand interface.
 */
class MVVM_MODEL_EXPORT AbstractCommand : public ICommand
{
public:
  AbstractCommand();
  ~AbstractCommand() override;

  AbstractCommand(const AbstractCommand&) = delete;
  AbstractCommand& operator=(const AbstractCommand&) = delete;
  AbstractCommand(AbstractCommand&&) = delete;
  AbstractCommand& operator=(AbstractCommand&&) = delete;

  void Execute() final;

  CommandStatus GetCommandStatus() const override;

  void Undo() final;

  bool IsObsolete() const override;

  void SetIsObsolete(bool value) override;

  std::string GetDescription() const override;

  void SetDescription(const std::string& text) const override;

private:
  virtual void ExecuteImpl() = 0;

  virtual void UndoImpl() = 0;

  struct AbstractCommandImpl;
  std::unique_ptr<AbstractCommandImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_ABSTRACT_COMMAND_H_
