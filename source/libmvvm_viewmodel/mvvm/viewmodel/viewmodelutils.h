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

#ifndef MVVM_VIEWMODEL_VIEWMODELUTILS_H
#define MVVM_VIEWMODEL_VIEWMODELUTILS_H

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/standardpresentationitems.h"
#include "mvvm/viewmodel_export.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

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
template <typename T = SessionItem>
const T* GetItemFromView(const ViewItem* view_item)
{
  if (auto presentation = dynamic_cast<const SessionItemPresentation*>(view_item->item());
      presentation)
  {
    return dynamic_cast<const T*>(presentation->GetItem());
  }

  return nullptr;
}

template <typename T>
std::vector<ViewItem*> FindViewsForItem(const ViewModelBase* view_model, const T* item)
{
  if (!item)
  {
    return {};
  }

  std::vector<ViewItem*> result;
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

//! Returns vector of Qt roles corresponding to given ItemDataRole.
MVVM_VIEWMODEL_EXPORT QVector<int> ItemRoleToQtRole(int role);

//! Returns text color for given item.
MVVM_VIEWMODEL_EXPORT QVariant TextColorRole(const SessionItem& item);

//! Returns check state role of given item.
MVVM_VIEWMODEL_EXPORT QVariant CheckStateRole(const SessionItem& item);

//! Returns decoration role for given item.
MVVM_VIEWMODEL_EXPORT QVariant DecorationRole(const SessionItem& item);

//! Returns tooltip role for given item.
MVVM_VIEWMODEL_EXPORT QVariant ToolTipRole(const SessionItem& item);

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODEL_VIEWMODELUTILS_H
