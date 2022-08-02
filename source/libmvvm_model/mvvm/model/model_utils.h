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

#ifndef MVVM_MODEL_MODEL_UTILS_H_
#define MVVM_MODEL_MODEL_UTILS_H_

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model_export.h>

#include <memory>
#include <vector>

namespace mvvm
{
class Path;
}

namespace mvvm::utils
{

//! Returns top items of the given type.
//! The top item is an item that is a child of an invisible root item.

template <typename T = SessionItem>
std::vector<T*> GetTopItems(const SessionModelInterface* model)
{
  if (!model)
  {
    throw RuntimeException("Undefined model");
  }
  return utils::CastItems<T>(model->GetRootItem()->GetAllItems());
}

//! Returns top item of the given type. If more than one item exists, return the first one.
//! The top item is an item that is a child of an invisible root item.

template <typename T = SessionItem>
T* GetTopItem(const SessionModelInterface* model)
{
  auto items = GetTopItems<T>(model);
  return items.empty() ? nullptr : items.front();
}

//! Returns all items in a tree of given type.

template <typename T = SessionItem>
std::vector<T*> FindItems(const SessionModelInterface* model)
{
  std::vector<T*> result;

  auto func = [&result](SessionItem* item)
  {
    if (auto concrete = dynamic_cast<T*>(item); concrete)
    {
      result.push_back(concrete);
    }
  };

  iterate(model->GetRootItem(), func);

  return result;
}

//! Constructs path to find given item. Item must belong to a model.
MVVM_MODEL_EXPORT Path PathFromItem(const SessionItem* item);

//! Returns item found in the model following given Path.
MVVM_MODEL_EXPORT SessionItem* ItemFromPath(const SessionModelInterface& model, const Path& path);

//! Copies a given item and inserts result into the model using provided parent and tag_index.
//! Returns pointer to just inserted item to the user. Internally performs deep copying with all
//! identifiers being regenerated.
MVVM_MODEL_EXPORT SessionItem* CopyItem(const SessionItem* item, SessionModelInterface* model,
                                        SessionItem* parent, const TagIndex& tag_index);

////! Populate empty model with content of target model using provided converter.
////! Serves as auxiliary function for model copying and cloning.
// MVVM_MODEL_EXPORT void PopulateEmptyModel(const JsonModelConverterInterface* converter,
//                                          const SessionModel& source, SessionModel& target);
//
////! Creates full deep copy of given model. All item's ID will be generated.
// template <typename T = SessionModel>
// std::unique_ptr<T> CreateCopy(const T& model)
//{
//  auto result = std::make_unique<T>();
//  auto converter = CreateModelCopyConverter();
//  PopulateEmptyModel(converter.get(), model, *result.get());
//  return result;
//}

////! Creates exact clone of given model. All item's ID will be preserved.
// template <typename T = SessionModel>
// std::unique_ptr<T> CreateClone(const T& model)
//{
//  auto result = std::make_unique<T>();
//  auto converter = CreateModelCloneConverter();
//  PopulateEmptyModel(converter.get(), model, *result.get());
//  return result;
//}

////! Moves item up (decrements row of the item). Works on children belonging to single tag.
// MVVM_MODEL_EXPORT void MoveUp(SessionItem* item);

////! Moves item down (increments row of the item). Works on children belonging to single tag.
// MVVM_MODEL_EXPORT void MoveDown(SessionItem* item);

//! Undo last model operation. If undo/redo is disabled, will do nothing.
 MVVM_MODEL_EXPORT void Undo(SessionModelInterface& model);

//! Redo model operation which was undone just before. If undo/redo is disabled, will do nothing.
 MVVM_MODEL_EXPORT void Redo(SessionModelInterface& model);

////! Begin undo/redo macros with given name. Works only if item belongs to the model, and model has
////! undo/redo enabled. Otherwise, do nothing.
// MVVM_MODEL_EXPORT void BeginMacros(const SessionItem* item, const std::string& macro_name);

////! Finishes undo/redo macros. Works only if item belongs to the model, and model has undo/redo
////! enabled. Otherwise, do nothing.
// MVVM_MODEL_EXPORT void EndMacros(const SessionItem* item);

////! Begin undo/redo macros with given name. Works only if the model has undo/redo enabled.
////! Otherwise, do nothing.
// MVVM_MODEL_EXPORT void BeginMacros(const SessionModel* model, const std::string& macro_name);

////! Finishes undo/redo macros. Works only if the model has undo/redo enabled. Otherwise, do
/// nothing.
// MVVM_MODEL_EXPORT void EndMacros(const SessionModel* model);

//! Returns empty root item.
MVVM_MODEL_EXPORT std::unique_ptr<SessionItem> CreateEmptyRootItem();

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_MODEL_UTILS_H_
