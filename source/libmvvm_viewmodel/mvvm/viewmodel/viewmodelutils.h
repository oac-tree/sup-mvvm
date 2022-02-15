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
template <typename T>
const T* GetItemFromView(const ViewItem* view_item)
{
  if (auto presentation = dynamic_cast<const SessionItemPresentation*>(view_item->item());
      presentation)
  {
    return dynamic_cast<const T*>(presentation->GetItem());
  }

  //  throw std::runtime_error("Can't cast presentation");

  return dynamic_cast<const T*>(GetContext<SessionItem>(view_item));
}

//! Returns vector of Qt roles corresponding to given ItemDataRole.
MVVM_VIEWMODEL_EXPORT QVector<int> ItemRoleToQtRole(int role);

//! Returns text color for given item.
MVVM_VIEWMODEL_EXPORT QVariant TextColorRole(const SessionItem& item);

//! Returns check state role of given item.
MVVM_VIEWMODEL_EXPORT QVariant CheckStateRole(const SessionItem& item);

////! Returns decoration role for given item.
// MVVM_VIEWMODEL_EXPORT QVariant DecorationRole(const SessionItem& item);

//! Returns tooltip role for given item.
MVVM_VIEWMODEL_EXPORT QVariant ToolTipRole(const SessionItem& item);

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODEL_VIEWMODELUTILS_H
