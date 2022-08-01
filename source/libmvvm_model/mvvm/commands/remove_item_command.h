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

#ifndef MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_
#define MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/variant.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class ModelComposerInterface;
class TagIndex;

//! Interface class for generic command.

class MVVM_MODEL_EXPORT RemoveItemCommand : public AbstractCommand
{
public:
  RemoveItemCommand(ModelComposerInterface* composer, SessionItem* parent, const TagIndex& tag_index);

  ~RemoveItemCommand() override;

  std::unique_ptr<SessionItem> GetResult() const;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;

  struct RemoveItemCommandImpl;
  std::unique_ptr<RemoveItemCommandImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_
