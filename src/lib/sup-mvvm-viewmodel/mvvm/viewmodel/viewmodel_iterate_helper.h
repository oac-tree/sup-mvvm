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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_ITERATE_HELPER_H_
#define MVVM_VIEWMODEL_VIEWMODEL_ITERATE_HELPER_H_

#include <QAbstractItemModel>
#include <functional>

namespace mvvm
{

//! Iterates through the QAbstractItem model and calls the user function on every iteration.
void iterate_model(const QAbstractItemModel* model, const QModelIndex& parent,
                   const std::function<void(const QModelIndex& child)>& fun);

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_ITERATE_HELPER_H_
