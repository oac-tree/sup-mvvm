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

#include "graphics_scene_helper.h"

#include "connectable_shape.h"
#include "i_connectable_shape_adapter.h"

#include <mvvm/model/session_item.h>

namespace mvvm
{

SessionItem *GetUnderlyingItem(ConnectableShape *shape)
{
  return shape->GetConnectableShapeAdapter()->GetItem();
}

}  // namespace mvvm
