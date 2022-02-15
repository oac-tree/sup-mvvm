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

#ifndef MVVM_VIEWMODEL_VIEWITEMFACTORY_H
#define MVVM_VIEWMODEL_VIEWITEMFACTORY_H

//! @file viewitemfactory.h
//! Contains factory methods to construct ViewItem's looking at different SessionItem parts.

#include "mvvm/viewmodel_export.h"

#include <memory>

namespace mvvm
{
class ViewItem;
class SessionItem;

//! Creates ViewItem playing the role of root.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateRootViewItem(SessionItem* item);

//! Creates read-only ViewItem for SessionItem's display name.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item);

//! Creates ViewItem for SessionItem's data.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item);

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWITEMFACTORY_H
