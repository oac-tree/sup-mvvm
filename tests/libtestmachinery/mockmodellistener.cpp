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

#include "mockmodellistener.h"

#include "mvvm/model/sessionitem.h"

void MockModelListener::SubscribeTo(mvvm::ModelEventSubscriberInterface *notifier)
{
  m_slot = std::make_unique<mvvm::Slot>();

  auto on_about_to_insert = [this](auto item, auto tagindex)
  { OnAboutToInsertItem(item, tagindex); };
  notifier->SetOnAboutToInsertItem(on_about_to_insert, m_slot.get());

  auto on_item_inserted = [this](auto item, auto tagindex) { OnItemInserted(item, tagindex); };
  notifier->SetOnItemInserted(on_item_inserted, m_slot.get());

  auto on_about_to_remove = [this](auto item, auto tagindex)
  { OnAboutToRemoveItem(item, tagindex); };
  notifier->SetOnAboutToRemoveItem(on_about_to_remove, m_slot.get());

  auto on_item_removed = [this](auto item, auto tagindex) { OnItemRemoved(item, tagindex); };
  notifier->SetOnItemRemoved(on_item_removed, m_slot.get());

  auto on_data_changed = [this](auto item, auto role) { OnDataChanged(item, role); };
  notifier->SetOnDataChanged(on_data_changed, m_slot.get());

  auto on_model_about_reset = [this](auto model) { OnModelAboutToBeReset(model); };
  notifier->SetOnModelAboutToBeReset(on_model_about_reset, m_slot.get());

  auto on_model_reset = [this](auto model) { OnModelReset(model); };
  notifier->SetOnModelReset(on_model_reset, m_slot.get());

  auto on_model_about_destroyed = [this](auto model) { OnModelAboutToBeDestroyed(model); };
  notifier->SeOnModelAboutToBeDestroyed(on_model_about_destroyed, m_slot.get());
}

void MockModelListener::UnsubscribeFrom()
{
  m_slot.reset();
}
