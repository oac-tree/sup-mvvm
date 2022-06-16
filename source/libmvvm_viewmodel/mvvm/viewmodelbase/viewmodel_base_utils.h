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

#ifndef MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H
#define MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H

#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/viewmodelbase/viewmodel_base.h>

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

//! Returns underlying presentation item casted to given type.

template <typename T>
const T* GetPresentation(const ViewItem* view_item)
{
  return dynamic_cast<const T*>(view_item->item());
}

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODELBASE_VIEWMODELBASEUTILS_H
