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

#ifndef MVVM_COMMANDS_ABSTRACTCOMMAND_H
#define MVVM_COMMANDS_ABSTRACTCOMMAND_H

#include "mvvm/commands/commandinterface.h"
#include "mvvm/model_export.h"

#include <memory>

namespace mvvm
{

//! Interface class for generic command.

class MVVM_MODEL_EXPORT AbstractCommand : public CommandInterface
{
public:
  AbstractCommand();
  ~AbstractCommand() override;

  void Execute() final;

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

#endif  // MVVM_COMMANDS_ABSTRACTCOMMAND_H
