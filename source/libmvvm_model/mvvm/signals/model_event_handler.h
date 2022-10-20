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

#ifndef MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_
#define MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_

#include <mvvm/signals/callback_types.h>
#include <mvvm/signals/event_handler.h>
#include <mvvm/viewmodel_export.h>

namespace mvvm
{
//! Provides notification for all subscribers when some event happened with SessionModel.

class MVVM_VIEWMODEL_EXPORT ModelEventHandler : public EventHandler
{
public:
  virtual Connection SetOnAboutToInsertItem(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnItemInserted(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnItemRemoved(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnDataChanged(const Callbacks::item_int_t& f, Slot* slot);

  virtual Connection SetOnModelAboutToBeReset(const Callbacks::model_t& f, Slot* slot);

  virtual Connection SetOnModelReset(const Callbacks::model_t& f, Slot* slot);

  virtual Connection SetOnModelAboutToBeDestroyed(const Callbacks::model_t& f, Slot* slot);

  // methods to notify

  virtual void AboutToInsertItemNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void ItemInsertedNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void AboutToRemoveItemNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void ItemRemovedNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void DataChangedNotify(SessionItem* item, int role);

  virtual void ModelAboutToBeResetNotify(SessionModelInterface* model);

  virtual void ModelResetNotify(SessionModelInterface* model);

  virtual void ModelAboutToBeDestroyedNotify(SessionModelInterface* model);
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_
