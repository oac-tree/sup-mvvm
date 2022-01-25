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

#ifndef MVVM_SIGNALS_MODELEVENTNOTIFIER_H
#define MVVM_SIGNALS_MODELEVENTNOTIFIER_H

#include "mvvm/interfaces/modeleventnotifierinterface.h"
#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/viewmodel_export.h"

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
  explicit ModelEventNotifier(ModelEventListenerInterface* listener = nullptr);
  ~ModelEventNotifier() override;

  void Unsubscribe(ModelEventListenerInterface* listener) override;

  void Subscribe(ModelEventListenerInterface* listener) override;

  // methods to subscribe for notifications

  Connection SetOnAboutToInsertItem(Callbacks::item_tagindex_t f, Slot* slot) override;

  Connection SetOnItemInserted(Callbacks::item_tagindex_t f, Slot* slot) override;

  Connection SetOnAboutToRemoveItem(Callbacks::item_tagindex_t f, Slot* slot) override;

  Connection SetOnItemRemoved(Callbacks::item_tagindex_t f, Slot* slot) override;

  Connection SetOnDataChanged(Callbacks::item_int_t f, Slot* slot) override;

  Connection SetOnModelAboutToBeReset(Callbacks::model_t f, Slot* slot) override;

  Connection SetOnModelReset(Callbacks::model_t f, Slot* slot) override;

  Connection SeOnModelAboutToBeDestroyed(Callbacks::model_t f, Slot* slot) override;

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

#endif  // MVVM_SIGNALS_MODELEVENTNOTIFIER_H
