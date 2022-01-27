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

#ifndef MVVM_INTERFACES_ITEMEVENTSUBSCRIBERINTERFACE_H
#define MVVM_INTERFACES_ITEMEVENTSUBSCRIBERINTERFACE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"
#include "mvvm/signals/callback_types.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
class SessionItem;
class TagIndex;
class SessionModel;

//! Interface to subscribe user callbacks to events happening with specified item.

class MVVM_MODEL_EXPORT ItemEventSubscriberInterface
{
public:
  //! Sets callback to be notified on item insert. The callback will be called with
  //! (SessionItem* parent, tag_index), where 'tag_index' denotes child position after insert.
  virtual Connection SetOnItemInserted(Callbacks::item_tagindex_t f, Slot* slot = nullptr) = 0;

  //! Sets callback to be notified when the item is about to be removed. The callback will be
  //! called with (SessionItem* parent, tag_index), where 'tag_index' denotes child position being
  //! removed.
  virtual Connection SetOnAboutToRemoveItem(Callbacks::item_tagindex_t f, Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item remove. The callback wil be called with
  //! (SessionItem* parent, tag_index), where 'tag_index' denotes child position before the removal.
  virtual Connection SetOnItemRemoved(Callbacks::item_tagindex_t f, Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item's data change. The callback will be called
  //! with (SessionItem*, data_role).
  virtual Connection SetOnDataChanged(Callbacks::item_int_t f, Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item's property change.
  //! The callback will be called with (compound_item, property_name).
  virtual Connection SetOnPropertyChanged(Callbacks::item_str_t f, Slot* slot = nullptr) = 0;

  //! Sets callback to be notified just before the item destruction.
  virtual Connection SetOnItemDestroy(Callbacks::item_t f, Slot* slot = nullptr) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODELEVENTSUBSCRIBERINTERFACE_H
