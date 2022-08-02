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

#include "mvvm/model/model_utils.h"

#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/factories/item_copy_strategy_factory.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/path.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/model/validate_utils.h>

namespace
{
//! Returns CommandStack from the model, or nullptr if the stack doesn't exist.
mvvm::CommandStackInterface* GetCommandStack(const mvvm::SessionModelInterface& model)
{
  if (auto application_model = dynamic_cast<const mvvm::ApplicationModel*>(&model);
      application_model)
  {
    return application_model->GetCommandStack();
  }
  return nullptr;
}
}  // namespace

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

SessionItem* ItemFromPath(const SessionModelInterface& model, const Path& path)
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

SessionItem* CopyItem(const SessionItem* item, SessionModelInterface* model, SessionItem* parent,
                      const TagIndex& tag_index)
{
  auto copy_strategy = CreateItemCopyStrategy(model->GetFactory());
  return model->InsertItem(copy_strategy->CreateCopy(item), parent, tag_index);
}

//! FIXME restore functionality PopulateEmptyModel json

// void Utils::PopulateEmptyModel(const JsonModelConverterInterface* converter,
//                               const SessionModel& source, SessionModel& target)
//{
//    QJsonObject object = converter->to_json(source);
//    converter->from_json(object, target);
//}

void MoveUp(SessionItem* item)
{
  auto tag_index = item->GetTagIndex();
  if (tag_index.index == 0)
  {
    return;  // item already at the top
  }
  item->GetModel()->MoveItem(item, item->GetParent(), tag_index.Prev());
}

void MoveDown(SessionItem* item)
{
  auto tag_index = item->GetTagIndex();
  if (tag_index.index == item->GetParent()->GetItemCount(tag_index.tag) - 1)
  {
    return;  // item already at the buttom
  }
  item->GetModel()->MoveItem(item, item->GetParent(), tag_index.Next());
}

void Undo(SessionModelInterface& model)
{
  if (auto command_stack = GetCommandStack(model); command_stack)
  {
    command_stack->Undo();
  }
}

void Redo(SessionModelInterface& model)
{
  if (auto command_stack = GetCommandStack(model); command_stack)
  {
    command_stack->Redo();
  }
}

//! FIXME restore beginMacros

// void Utils::BeginMacros(const SessionItem* item, const std::string& macro_name)
//{
//    if (!item)
//        return;
//    BeginMacros(item->model(), macro_name);
//}

// void Utils::EndMacros(const SessionItem* item)
//{
//    if (!item)
//        return;
//    EndMacros(item->model());
//}

// void Utils::BeginMacros(const SessionModel* model, const std::string& macro_name)
//{
//    if (!model)
//        return;
//    if (auto stack = model->undoStack(); stack)
//        stack->beginMacro(macro_name);
//}

//! FIXME restore endMacros

// void Utils::EndMacros(const SessionModel* model)
//{
//    if (!model)
//        return;
//    if (auto stack = model->undoStack(); stack)
//        stack->endMacro();
//}

std::unique_ptr<SessionItem> CreateEmptyRootItem()
{
  auto result = std::make_unique<SessionItem>();
  result->RegisterTag(TagInfo::CreateUniversalTag("rootTag"), /*set_as_default*/ true);
  return result;
}

}  // namespace mvvm::utils
