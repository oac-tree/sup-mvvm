/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H
#define MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H

#include "mvvm/viewmodel/standardpresentationitems.h"
#include "mvvm/viewmodelbase/presentationitem.h"
#include "mvvm/viewmodelbase/viewitem.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"

#include <QAbstractItemModel>
#include <functional>

namespace mvvm
{
class ViewModelBase;
}

namespace mvvm::utils
{
//! Iterates through the QAbstractItem model and calls the user function on every iteration.
MVVM_VIEWMODEL_EXPORT void iterate_model(const QAbstractItemModel* model, const QModelIndex& parent,
                                         const std::function<void(const QModelIndex& child)>& fun);

//! Returns underlying presentation item PresentationItem<Instruction>, if underlying
//! SessionItemData can be cast to that. Returns nullptr otherwise.

template <typename T>
const PresentationItem<T>* GetPresentation(const ViewItem* view_item)
{
  return dynamic_cast<const PresentationItem<T>*>(view_item->item());
}

//! FIXME refactor GetContext and GetItem method, they are errorprone
//! The result depend on the way PresentationItem has been created
//! See TEST_F(ViewModelBaseUtilsTest, GetItem) viewmodelbaseutils.test.cpp

//! Returns context (SessionItem) from underlying presentation item.

template <typename T>
const T* GetContext(const ViewItem* view_item)
{
  if (auto presentation = GetPresentation<T>(view_item); presentation)
  {
    return dynamic_cast<const T*>(presentation->GetContext());
  }

  // FIXME refactor
  if (auto presentation = dynamic_cast<const SessionItemPresentation*>(view_item->item());
      presentation)
  {
    return dynamic_cast<const T*>(presentation->GetItem());
  }

  return nullptr;
}

//! Finds ViewItems in given ViewModelBase representing given context.

template <typename T>
std::vector<ViewItem*> FindViews(const ViewModelBase* view_model, const T* item)
{
  if (!item)
  {
    return {};
  }

  std::vector<ViewItem*> result;
  if (item == GetContext<T>(view_model->rootItem()))
  {
    result.push_back(view_model->rootItem());
  }

  auto on_index = [&](const QModelIndex& index)
  {
    auto view_item = view_model->itemFromIndex(index);
    if (GetContext<T>(view_item) == item)
    {
      result.push_back(view_item);
    }
  };
  utils::iterate_model(view_model, QModelIndex(), on_index);
  return result;
}

//! FIXME remove code

//! Creates presentation to show the label associated with the given context.
//! Context itself is not used directly. It might be used by ViewModelController to find
//! corresponding views.

// template <typename T>
// std::unique_ptr<ViewItemDataInterface> CreateLabelPresentation(T* context, const std::string&
// label)
//{
//   // label is simply copied
//   auto on_data = [label](T* instruction, int role)
//   { return role == Qt::DisplayRole ? QString::fromStdString(label) : QVariant(); };

//  return std::make_unique<PresentationItem<T>>(context, on_data);
//}

//! FIXME remove code

// template <typename T>
// std::unique_ptr<ViewItem> CreateLabelViewItem(T* context, const std::string& label)
//{
//   return std::make_unique<ViewItem>(CreateLabelPresentation(context, label));
// }

//! Creates ViewItem representing invisible root item.

//! FIXME remove code

// template <typename T>
// std::unique_ptr<ViewItem> CreateRootViewItem(T* context)
//{
//   // it has context which can't be edited
//   auto presentation = std::make_unique<PresentationItem<T>>(context);
//   return std::make_unique<ViewItem>(std::move(presentation));
// }

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H
