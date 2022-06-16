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

#ifndef MVVM_SIGNALS_MODEL_EVENT_NOTIFIER_H_
#define MVVM_SIGNALS_MODEL_EVENT_NOTIFIER_H_

#include <mvvm/interfaces/model_event_notifier_interface.h>
#include <mvvm/interfaces/model_event_subscriber_interface.h>
#include <mvvm/viewmodel_export.h>

#include <map>
#include <memory>
#include <vector>

namespace mvvm
{
//! Provides notification for all subscribers when some event happened with SessionModel.

class MVVM_VIEWMODEL_EXPORT ModelEventNotifier : public ModelEventNotifierInterface,
                                                 public ModelEventSubscriberInterface
{
public:
  explicit ModelEventNotifier();
  ~ModelEventNotifier() override;

  // methods to subscribe for notifications

  Connection SetOnAboutToInsertItem(const Callbacks::item_tagindex_t& f, Slot* slot) override;

  Connection SetOnItemInserted(const Callbacks::item_tagindex_t& f, Slot* slot) override;

  Connection SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t& f, Slot* slot) override;

  Connection SetOnItemRemoved(const Callbacks::item_tagindex_t& f, Slot* slot) override;

  Connection SetOnDataChanged(const Callbacks::item_int_t& f, Slot* slot) override;

  Connection SetOnModelAboutToBeReset(const Callbacks::model_t& f, Slot* slot) override;

  Connection SetOnModelReset(const Callbacks::model_t& f, Slot* slot) override;

  Connection SetOnModelAboutToBeDestroyed(const Callbacks::model_t& f, Slot* slot) override;

  // methods to trigger notifications
  // FIXME consider making private inheritance from ModelEventSubscriberInterface, and making
  // ApplicationModel a friend

  void AboutToInsertItemNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemInsertedNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void AboutToRemoveItemNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemRemovedNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void DataChangedNotify(SessionItem* item, int role) override;

  void ModelAboutToBeResetNotify(SessionModel* model) override;

  void ModelResetNotify(SessionModel* model) override;

  void ModelAboutToBeDestroyedNotify(SessionModel* model) override;

private:
  struct ModelEventNotifierImpl;
  std::unique_ptr<ModelEventNotifierImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_EVENT_NOTIFIER_H_
