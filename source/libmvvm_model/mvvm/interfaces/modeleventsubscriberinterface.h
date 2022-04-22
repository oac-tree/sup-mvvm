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

#ifndef MVVM_INTERFACES_MODELEVENTSUBSCRIBERINTERFACE_H
#define MVVM_INTERFACES_MODELEVENTSUBSCRIBERINTERFACE_H

#include "mvvm/model_export.h"
#include "mvvm/signals/callback_types.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
class SessionItem;
class TagIndex;
class SessionModel;

//! Interface to subscribe user callbacks to events happening with the model.

class MVVM_MODEL_EXPORT ModelEventSubscriberInterface
{
public:
  //! Sets callback to be notified when the item is about to be inserted. The callback will be
  //! called with (SessionItem* parent, tag_index), where 'tag_index' denotes child position.
  virtual Connection SetOnAboutToInsertItem(const Callbacks::item_tagindex_t& f,
                                            Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item insert. The callback will be called with
  //! (SessionItem* parent, tag_index), where 'tag_index' denotes child position after insert.
  virtual Connection SetOnItemInserted(const Callbacks::item_tagindex_t& f,
                                       Slot* slot = nullptr) = 0;

  //! Sets callback to be notified when the item is about to be removed. The callback will be
  //! called with (SessionItem* parent, tag_index), where 'tag_index' denotes child position being
  //! removed.
  virtual Connection SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t& f,
                                            Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item remove. The callback will be called with
  //! (SessionItem* parent, tag_index), where 'tag_index' denotes child position before the removal.
  virtual Connection SetOnItemRemoved(const Callbacks::item_tagindex_t& f,
                                      Slot* slot = nullptr) = 0;

  //! Sets callback to be notified on item's data change. The callback will be called
  //! with (SessionItem*, data_role).
  virtual Connection SetOnDataChanged(const Callbacks::item_int_t& f, Slot* slot = nullptr) = 0;

  //! Sets the callback to be notified just before the reset of the root item.
  virtual Connection SetOnModelAboutToBeReset(const Callbacks::model_t& f,
                                              Slot* slot = nullptr) = 0;

  //! Sets the callback to be notified right after the root item recreation.
  virtual Connection SetOnModelReset(const Callbacks::model_t& f, Slot* slot = nullptr) = 0;

  //! Sets the callback for notifications just before model destruction.
  virtual Connection SetOnModelAboutToBeDestroyed(const Callbacks::model_t& f,
                                                  Slot* slot = nullptr) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODELEVENTSUBSCRIBERINTERFACE_H
