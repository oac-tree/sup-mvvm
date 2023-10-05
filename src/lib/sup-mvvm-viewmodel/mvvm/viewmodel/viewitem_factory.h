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

#ifndef MVVM_VIEWMODEL_VIEWITEM_FACTORY_H_
#define MVVM_VIEWMODEL_VIEWITEM_FACTORY_H_

//! @file viewitem_factory.h
//! Contains factory methods to construct ViewItem's looking at different SessionItem parts.

#include <mvvm/model/mvvm_types.h>
#include <mvvm/viewmodel_export.h>

#include <memory>
#include <string>

namespace mvvm
{
class ViewItem;
class SessionItem;

//! Creates ViewItem playing the role of root.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateRootViewItem(SessionItem* item);

//! Creates read-only ViewItem displaying a label.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateLabelViewItem(SessionItem* item,
                                                                    const std::string& label = {});

//! Creates read-only ViewItem for SessionItem's display name.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item);

//! Creates ViewItem for SessionItem's data.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item,
                                                                   int role = DataRole::kData);

//! Creates editable ViewItem for SessionItem's display name.
MVVM_VIEWMODEL_EXPORT std::unique_ptr<ViewItem> CreateEditableDisplayNameViewItem(
    SessionItem* item);

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWITEM_FACTORY_H_
