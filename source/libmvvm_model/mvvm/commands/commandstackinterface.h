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

#ifndef MVVM_COMMANDS_COMMANDSTACKINTERFACE_H
#define MVVM_COMMANDS_COMMANDSTACKINTERFACE_H

#include "mvvm/model_export.h"

#include <memory>

namespace mvvm
{

class AbstractCommand;

//! Interface class for item construction and item registration.

class MVVM_MODEL_EXPORT CommandStackInterface
{
public:
  virtual ~CommandStackInterface() = default;

  virtual void Execute(std::unique_ptr<AbstractCommand> command) = 0;

  virtual bool CanUndo() const = 0;
  virtual bool CanRedo() const = 0;
  virtual int GetIndex() const = 0;
  virtual int GetSize() const = 0;
  virtual void Undo() = 0;
  virtual void Redo() = 0;
  virtual void Clear() = 0;
  virtual void SetUndoLimit(int limit) = 0;

  virtual void BeginMacro(const std::string& name) = 0;
  virtual void EndMacro() = 0;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMANDSTACKINTERFACE_H
