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

#include "abstract_viewmodel_controller.h"

#include "mvvm/signals/model_event_handler.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm
{
AbstractViewModelController::~AbstractViewModelController() = default;

void AbstractViewModelController::SubscribeTo(ModelEventHandler *event_handler)
{
  if (!event_handler)
  {
    throw RuntimeException("Subscriber is not initialised");
  }

  m_slot = std::make_unique<mvvm::Slot>();

  auto on_about_to_insert = [this](auto item, auto tagindex)
  { OnAboutToInsertItem(item, tagindex); };
  event_handler->SetOnAboutToInsertItem(on_about_to_insert, m_slot.get());

  auto on_item_inserted = [this](auto item, auto tagindex) { OnItemInserted(item, tagindex); };
  event_handler->SetOnItemInserted(on_item_inserted, m_slot.get());

  auto on_about_to_remove = [this](auto item, auto tagindex)
  { OnAboutToRemoveItem(item, tagindex); };
  event_handler->SetOnAboutToRemoveItem(on_about_to_remove, m_slot.get());

  auto on_item_removed = [this](auto item, auto tagindex) { OnItemRemoved(item, tagindex); };
  event_handler->SetOnItemRemoved(on_item_removed, m_slot.get());

  auto on_data_changed = [this](auto item, auto role) { OnDataChanged(item, role); };
  event_handler->SetOnDataChanged(on_data_changed, m_slot.get());

  auto on_model_about_reset = [this](auto model) { OnModelAboutToBeReset(model); };
  event_handler->SetOnModelAboutToBeReset(on_model_about_reset, m_slot.get());

  auto on_model_reset = [this](auto model) { OnModelReset(model); };
  event_handler->SetOnModelReset(on_model_reset, m_slot.get());

  auto on_model_about_destroyed = [this](auto model) { OnModelAboutToBeDestroyed(model); };
  event_handler->SetOnModelAboutToBeDestroyed(on_model_about_destroyed, m_slot.get());
}

void AbstractViewModelController::UnsubscribeFrom(ModelEventHandler *event_handler)
{
  // FIXME Do we need Unsubscribe methods?
  (void)event_handler;
  m_slot.reset();
}

void AbstractViewModelController::OnAboutToInsertItem(SessionItem *parent,
                                                      const TagIndex &tag_index)
{
}

void AbstractViewModelController::OnItemInserted(SessionItem *parent, const TagIndex &tag_index) {}

void AbstractViewModelController::OnAboutToRemoveItem(SessionItem *parent,
                                                      const TagIndex &tag_index)
{
}

void AbstractViewModelController::OnItemRemoved(SessionItem *parent, const TagIndex &tag_index) {}

void AbstractViewModelController::OnDataChanged(SessionItem *item, int role) {}

void AbstractViewModelController::OnModelAboutToBeReset(SessionModelInterface *model) {}

void AbstractViewModelController::OnModelReset(SessionModelInterface *model) {}

void AbstractViewModelController::OnModelAboutToBeDestroyed(SessionModelInterface *model) {}

void AbstractViewModelController::Init(SessionItem *) {}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

}  // namespace mvvm
