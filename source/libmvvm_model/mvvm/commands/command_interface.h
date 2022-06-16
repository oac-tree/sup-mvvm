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

#ifndef MVVM_COMMANDS_COMMAND_INTERFACE_H_
#define MVVM_COMMANDS_COMMAND_INTERFACE_H_

#include <mvvm/model_export.h>

#include <string>
#include <vector>

namespace mvvm
{

//! Interface class for generic command.

class MVVM_MODEL_EXPORT CommandInterface
{
public:
  virtual ~CommandInterface() = default;

  //! Execute the command.
  virtual void Execute() = 0;

  //! Undo command as it was before execution.
  virtual void Undo() = 0;

  //! Returns whether the command is obsolete (which means that it shouldn't be kept in the stack).
  virtual bool IsObsolete() const = 0;

  //! Sets obsolete flag.
  virtual void SetIsObsolete(bool value) = 0;

  //! Returns the command description.
  virtual std::string GetDescription() const = 0;

  //! Set command description.
  virtual void SetDescription(const std::string& text) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_INTERFACE_H_
