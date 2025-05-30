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

#include "model_utils.h"

#include "application_model.h"
#include "item_constants.h"
#include "path.h"
#include "taginfo.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm::utils
{
Path PathFromItem(const SessionItem* item)
{
  if (!item || !item->GetModel())
  {
    return {};
  }

  Path result;
  const SessionItem* current(item);
  while (current && current->GetParent())
  {
    result.Prepend(utils::IndexOfChild(current->GetParent(), current));
    current = current->GetParent();
  }
  return result;
}

SessionItem* ItemFromPath(const ISessionModel& model, const Path& path)
{
  SessionItem* result(model.GetRootItem());
  for (const auto& x : path)
  {
    result = utils::ChildAt(result, x);
    if (!result)
    {
      break;
    }
  }
  return result;
}

SessionItem* CopyItem(const SessionItem* item, ISessionModel* model, SessionItem* parent,
                      const TagIndex& tag_index)
{
  return model->InsertItem(CopyItem(*item), parent, tag_index);
}

std::unique_ptr<SessionItem> CreateEmptyRootItem()
{
  auto result = std::make_unique<SessionItem>();
  // root item is invisible in widgets, so display name is for debugging purpose
  result->SetDisplayName(constants::kRootItemName);
  result->RegisterTag(TagInfo::CreateUniversalTag(constants::kRootItemTag),
                      /*set_as_default*/ true);
  return result;
}

void Undo(ISessionModel& model)
{
  if (auto command_stack = model.GetCommandStack(); command_stack)
  {
    command_stack->Undo();
  }
}

void Redo(ISessionModel& model)
{
  if (auto command_stack = model.GetCommandStack(); command_stack)
  {
    command_stack->Redo();
  }
}

void BeginMacro(const ISessionModel& model, const std::string& macro_name)
{
  if (auto command_stack = model.GetCommandStack(); command_stack)
  {
    command_stack->BeginMacro(macro_name);
  }
}

void EndMacro(const ISessionModel& model)
{
  if (auto command_stack = model.GetCommandStack(); command_stack)
  {
    command_stack->EndMacro();
  }
}

}  // namespace mvvm::utils
