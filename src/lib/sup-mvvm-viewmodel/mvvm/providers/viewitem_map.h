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

#ifndef MVVM_PROVIDERS_VIEWITEM_MAP_H_
#define MVVM_PROVIDERS_VIEWITEM_MAP_H_

#include <mvvm/providers/item_view_map.h>

namespace mvvm
{

class ViewItem;
class SessionItem;

/**
 * @brief The ViewItemMap class stores correspondance of the SessionItem and ViewItem. Plays a
 * supporting role during ViewModel rebuild.
 */
class ViewItemMap : public ItemViewMap<SessionItem, ViewItem>
{
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWITEM_MAP_H_
