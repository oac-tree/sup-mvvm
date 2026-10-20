/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PROVIDERS_VIEWITEM_FACTORY_H_
#define MVVM_PROVIDERS_VIEWITEM_FACTORY_H_

//! @file
//! Contains factory methods to construct ViewItem's looking at different SessionItem parts.

#include <mvvm/core/mvvm_types.h>

#include <map>
#include <memory>
#include <string>

class QVariant;

namespace mvvm
{

class ViewItem;
class SessionItem;

/**
 * @brief Creates ViewItem playing the role of root.
 */
std::unique_ptr<ViewItem> CreateRootViewItem(SessionItem* item);

/**
 * @brief Creates read-only ViewItem displaying a label.
 */
std::unique_ptr<ViewItem> CreateLabelViewItem(SessionItem* item, const std::string& label = {});

/**
 * @brief Creates ViewItem for SessionItem's display name.
 *
 * Display name will be editable, if item has attribute Appearance::kEditableDisplayName and is
 * editable itself. Otherwise, display name will be read-only.
 */
std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item);

/**
 * @brief Creates ViewItem for SessionItem's data.
 *
 * @details Bool data gets a checkbox, unless the item has Appearance::kNullable.
 */
std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item, role_t role = DataRole::kData);

/**
 * @brief Creates editable ViewItem for SessionItem's display name.
 *
 * Item display name will be editable, even if item doesn't have Appearance::kEditableDisplayName
 * attribute or is not editable itself.
 */
std::unique_ptr<ViewItem> CreateEditableDisplayNameViewItem(SessionItem* item);

/**
 * @brief Creates ViewItem for SessionItem's with arbitrary data for any Qt role.
 *
 * The data of item doesn't have any connection with SessionItem signals.
 */
std::unique_ptr<ViewItem> CreateFixedDataViewItem(SessionItem* item,
                                                  std::map<std::int32_t, QVariant> fixed_data = {});

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWITEM_FACTORY_H_
