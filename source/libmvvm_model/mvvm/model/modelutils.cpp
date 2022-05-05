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

#include "mvvm/model/modelutils.h"

#include "mvvm/factories/itemcopystrategyfactory.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/path.h"

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

SessionItem* ItemFromPath(const SessionModel& model, const Path& path)
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

bool HasSignals(const SessionModel* model)
{
  return dynamic_cast<const ApplicationModel*>(model);
}

void CopyItem(const SessionItem* item, SessionModel* model, SessionItem* parent,
              const TagIndex& tag_index)
{
  auto copy_strategy = CreateItemCopyStrategy(model->GetFactory());
  model->InsertItem(copy_strategy->CreateCopy(item), parent, tag_index);
}

//! FIXME restore functionality PopulateEmptyModel json

// void Utils::PopulateEmptyModel(const JsonModelConverterInterface* converter,
//                               const SessionModel& source, SessionModel& target)
//{
//    QJsonObject object = converter->to_json(source);
//    converter->from_json(object, target);
//}

//! FIXME restore MoveUp

// void Utils::MoveUp(SessionItem* item)
//{
//    auto tagrow = item->tagRow();
//    if (tagrow.row == 0)
//        return; // item already at the top
//    item->model()->moveItem(item, item->parent(), tagrow.prev());
//}

//! FIXME restore MoveDown

// void Utils::MoveDown(SessionItem* item)
//{
//    auto tagrow = item->tagRow();
//    if (tagrow.row == item->parent()->itemCount(tagrow.tag) - 1)
//        return; // item already at the buttom
//    item->model()->moveItem(item, item->parent(), tagrow.next());
//}

//! FIXME restore Undo

// void Utils::Undo(SessionModel& model)
//{
//    if (auto stack = model.undoStack(); stack)
//        stack->undo();
//}

//! FIXME restore Redo

// void Utils::Redo(SessionModel& model)
//{
//    if (auto stack = model.undoStack(); stack)
//        stack->redo();
//}

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

}  // namespace mvvm::utils
