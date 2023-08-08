/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

/**
 * @brief Returns top items of the given type.
 *
 * @details The top item is an item that is a child of an invisible root item.
 */
template <typename T = SessionItem>
std::vector<T*> GetTopItems(const SessionModelInterface* model)
{
  if (!model)
  {
    throw RuntimeException("Undefined model");
  }
  return utils::CastItems<T>(model->GetRootItem()->GetAllItems());
}

/**
 * @brief Returns top item of the given type.
 *
 * @details If more than one item exists, return the first one. The top item is an item that is a
 * child of an invisible root item.
 */
template <typename T = SessionItem>
T* GetTopItem(const SessionModelInterface* model)
{
  auto items = GetTopItems<T>(model);
  return items.empty() ? nullptr : items.front();
}

/**
 * @brief Returns all items in a tree of given type.
 */
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

/**
 * @brief Constructs path to find given item. Item must belong to a model.
 */
MVVM_MODEL_EXPORT Path PathFromItem(const SessionItem* item);

/**
 * @brief Returns item found in the model following given Path.
 */
MVVM_MODEL_EXPORT SessionItem* ItemFromPath(const SessionModelInterface& model, const Path& path);

/**
 * @brief Copies a given item and inserts result into the model using provided parent and tag_index.
 *
 * @param item An item to copy.
 * @param model The model to which the copy will belong to.
 * @param parent New parent of a copy.
 * @param tag_index TagIndex where to insert a new copy.
 *
 * @return Returns pointer to just inserted item to the user.
 */
MVVM_MODEL_EXPORT SessionItem* CopyItem(const SessionItem* item, SessionModelInterface* model,
                                        SessionItem* parent, const TagIndex& tag_index);

/**
 * @brief Undo last model operation. If undo/redo is disabled, will do nothing.
 */
MVVM_MODEL_EXPORT void Undo(SessionModelInterface& model);

/**
 * @brief Redo last model operation. If undo/redo is disabled, will do nothing.
 */
MVVM_MODEL_EXPORT void Redo(SessionModelInterface& model);

/**
 * @brief Creates new empty root item.
 */
MVVM_MODEL_EXPORT std::unique_ptr<SessionItem> CreateEmptyRootItem();

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_MODEL_UTILS_H_
