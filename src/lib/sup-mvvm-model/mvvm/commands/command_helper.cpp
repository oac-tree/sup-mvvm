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

#include "command_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

#include <sstream>

namespace mvvm
{

event_variant_t GetBeforeEventForInsertCommand(CommandStatus status, SessionItem *parent,
                                               const TagIndex &tag_index)
{
  if (status == CommandStatus::Initial || status == CommandStatus::AfterUndo)
  {
    return AboutToInsertItemEvent{parent, tag_index};
  }

  return AboutToRemoveItemEvent{parent, tag_index};
}

event_variant_t GetAfterEventForInsertCommand(CommandStatus status, SessionItem *parent,
                                              const TagIndex &tag_index)
{
  if (status == CommandStatus::AfterExecute)
  {
    return ItemInsertedEvent{parent, tag_index};
  }

  if (status == CommandStatus::AfterUndo)
  {
    return ItemRemovedEvent{parent, tag_index};
  }

  throw RuntimeException("Command can't be in initial state after execute or undo call");
}

std::string GetInsertCommandDescription(SessionItem *parent, SessionItem *child,
                                        const TagIndex &tag_index)
{
  std::ostringstream ostr;
  const std::string parent_name = parent ? parent->GetDisplayName() : "nullptr";
  const std::string child_name = child ? child->GetDisplayName() : "nullptr";
  ostr << "Insert: " << parent_name << " " << child_name << " " << tag_index.GetTag() << " "
       << tag_index.GetIndex();
  return ostr.str();
}

std::string GetSetValueCommandDescription(const variant_t &value, int role)
{
  std::ostringstream ostr;
  ostr << "Set value: " << mvvm::utils::ValueToString(value) << ", role:" << role;
  return ostr.str();
}

}  // namespace mvvm
