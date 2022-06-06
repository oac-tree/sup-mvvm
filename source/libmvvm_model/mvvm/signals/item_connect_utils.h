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

#ifndef MVVM_SIGNALS_ITEMCONNECTUTILS_H
#define MVVM_SIGNALS_ITEMCONNECTUTILS_H

//! @file itemconnectutils.h
//! Utility functions to connect user callbacks with events happening with given item.

#include "mvvm/model_export.h"
#include "mvvm/signals/callback_types.h"
#include "mvvm/signals/signal_slot.h"

namespace mvvm
{
class SessionItem;
}

namespace mvvm::connect
{

//! Sets callback to be notified on item insert. The callback will be called with
//! (SessionItem* parent, tag_index), where 'tag_index' denotes child position after insert.
MVVM_MODEL_EXPORT Connection OnItemInserted(SessionItem* source,
                                            const Callbacks::item_tagindex_t& func,
                                            Slot* slot = nullptr);

//! Sets callback to be notified when the item is about to be removed. The callback will be
//! called with (SessionItem* parent, tag_index), where 'tag_index' denotes child position being
//! removed.
MVVM_MODEL_EXPORT Connection OnAboutToRemoveItem(SessionItem* source,
                                                 const Callbacks::item_tagindex_t& func,
                                                 Slot* slot = nullptr);

//! Sets callback to be notified on item remove. The callback wil be called with
//! (SessionItem* parent, tag_index), where 'tag_index' denotes child position before the removal.
MVVM_MODEL_EXPORT Connection OnItemRemoved(SessionItem* source,
                                           const Callbacks::item_tagindex_t& func,
                                           Slot* slot = nullptr);

//! Sets callback to be notified on item's data change. The callback will be called
//! with (SessionItem*, data_role).
MVVM_MODEL_EXPORT Connection OnDataChanged(SessionItem* source, const Callbacks::item_int_t& func,
                                           Slot* slot = nullptr);

//! Sets callback to be notified on item's property change.
//! The callback will be called with (compound_item, property_name).
MVVM_MODEL_EXPORT Connection OnPropertyChanged(SessionItem* source,
                                               const Callbacks::item_str_t& func,
                                               Slot* slot = nullptr);

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEMCONNECTUTILS_H
