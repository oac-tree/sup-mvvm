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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_VIEWMODEL_UTILS_H_
#define MVVM_VIEWMODEL_VIEWMODEL_UTILS_H_

#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/standard_presentation_items.h>
#include <mvvm/viewmodel_export.h>
#include <mvvm/viewmodelbase/viewmodel_base_utils.h>

#include <QVariant>
#include <QVector>
#include <stdexcept>

namespace mvvm
{
class SessionItem;
class ViewItem;
}  // namespace mvvm

namespace mvvm::utils
{

/**
 * @brief Returns an item served by given view.
 */
template <typename T = SessionItem>
const T* GetItemFromView(const ViewItem* view_item)
{
  if (auto presentation = dynamic_cast<const SessionItemPresentation*>(view_item->GetItemData());
      presentation)
  {
    return dynamic_cast<const T*>(presentation->GetItem());
  }

  return nullptr;
}

/**
 * @brief Returns an item served by given view.
 */
template <typename T = SessionItem>
T* GetItemFromView(ViewItem* view_item)
{
  if (auto presentation = dynamic_cast<SessionItemPresentation*>(view_item->GetItemData()); presentation)
  {
    return dynamic_cast<T*>(presentation->GetItem());
  }

  return nullptr;
}

/**
 * @brief Returns vector of views for given item.
 */
template <typename T>
std::vector<const ViewItem*> FindViewsForItem(const ViewModelBase* view_model, const T* item)
{
  if (!item)
  {
    return {};
  }

  std::vector<const ViewItem*> result;
  if (item == GetItemFromView<T>(view_model->rootItem()))
  {
    result.push_back(view_model->rootItem());
  }

  auto on_index = [&](const QModelIndex& index)
  {
    auto view_item = view_model->itemFromIndex(index);
    if (GetItemFromView<T>(view_item) == item)
    {
      result.push_back(view_item);
    }
  };
  utils::iterate_model(view_model, QModelIndex(), on_index);
  return result;
}

/**
 * @brief Returns vector of Qt roles corresponding to given ItemDataRole.
 */
MVVM_VIEWMODEL_EXPORT QVector<int> ItemRoleToQtRole(int role);

/**
 * @brief Returns text color for given item.
 */
MVVM_VIEWMODEL_EXPORT QVariant TextColorRole(const SessionItem& item);

/**
 * @brief Returns check state role of given item.
 * @param item
 * @return
 */
MVVM_VIEWMODEL_EXPORT QVariant CheckStateRole(const SessionItem& item);

/**
 * @brief Returns decoration role for given item.
 */
MVVM_VIEWMODEL_EXPORT QVariant DecorationRole(const SessionItem& item);

/**
 * @brief Returns tooltip role for given item.
 */
MVVM_VIEWMODEL_EXPORT QVariant ToolTipRole(const SessionItem& item);

/**
 * @brief Returns underlying SessionItem's for given index.
 */
MVVM_VIEWMODEL_EXPORT SessionItem* ItemFromIndex(const QModelIndex& index);

/**
 * @brief Returns vector of underlying SessionItem's for given index list.
 */
MVVM_VIEWMODEL_EXPORT std::vector<SessionItem*> ItemsFromIndex(const QModelIndexList& index_list);

/**
 * @brief Returns vector of parent items from given index list.
 *
 * @details Finds all SessionItems corresponding to given index list and collect their parents.
 * Function is usefull in the context of table-like views when we want to find compound items (i.e.
 * Layers) from table cells containing LayerItem's properties (i.e. thickness).
 */
MVVM_VIEWMODEL_EXPORT std::vector<SessionItem*> ParentItemsFromIndex(
    const QModelIndexList& index_list);

/**
 * @brief Returns vector of Qt roles corresponding to the given item role.
 */
QVector<int> GetQtRoles(const ViewItem *view, int item_role);

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODEL_VIEWMODEL_UTILS_H_
