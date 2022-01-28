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

#ifndef MVVM_SIGNALS_ITEMCONNECTUTILS_H
#define MVVM_SIGNALS_ITEMCONNECTUTILS_H

#include "mvvm/model_export.h"
#include "mvvm/signals/callback_types.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
class SessionItem;
}

namespace mvvm::connect
{

//! Sets callback to be notified on item's data change. The callback will be called
//! with (SessionItem*, data_role).
MVVM_MODEL_EXPORT Connection OnDataChange(SessionItem* source, const Callbacks::item_int_t& func,
                                          Slot* slot = nullptr);

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEMCONNECTUTILS_H
