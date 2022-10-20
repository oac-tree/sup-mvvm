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

#include <mvvm/viewmodel_export.h>
#include <mvvm/signals/callback_types.h>
#include <mvvm/signals/signal_slot.h>
#include <map>
#include <memory>
#include <vector>

namespace mvvm
{
//! Provides notification for all subscribers when some event happened with SessionModel.

class MVVM_VIEWMODEL_EXPORT ModelEventHandler
{
public:
  explicit ModelEventHandler();
  virtual ~ModelEventHandler();

  // methods to subMVVM_SIGNALS_MODEL_EVENT_HANDLER_H_scribe for notifications

  virtual Connection SetOnAboutToInsertItem(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnItemInserted(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnItemRemoved(const Callbacks::item_tagindex_t& f, Slot* slot);

  virtual Connection SetOnDataChanged(const Callbacks::item_int_t& f, Slot* slot);

  virtual Connection SetOnModelAboutToBeReset(const Callbacks::model_t& f, Slot* slot);

  virtual Connection SetOnModelReset(const Callbacks::model_t& f, Slot* slot);

  virtual Connection SetOnModelAboutToBeDestroyed(const Callbacks::model_t& f, Slot* slot);

  // methods to trigger notifications
  // FIXME consider making private inheritance from ModelEventSubscriberInterface, and making
  // ApplicationModel a friend

  virtual void AboutToInsertItemNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void ItemInsertedNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void AboutToRemoveItemNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void ItemRemovedNotify(SessionItem* parent, const TagIndex& tag_index);

  virtual void DataChangedNotify(SessionItem* item, int role);

  virtual void ModelAboutToBeResetNotify(SessionModelInterface* model);

  virtual void ModelResetNotify(SessionModelInterface* model);

  virtual void ModelAboutToBeDestroyedNotify(SessionModelInterface* model);

private:
  struct ModelEventNotifierImpl;
  std::unique_ptr<ModelEventNotifierImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_
