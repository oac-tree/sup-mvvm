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

#include <mvvm/signals/model_event_handler.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm
{
AbstractViewModelController::~AbstractViewModelController() = default;

void AbstractViewModelController::Subscribe(ModelEventHandler *event_handler)
{
  if (!event_handler)
  {
    throw RuntimeException("Subscriber is not initialised");
  }

  m_slot = std::make_unique<mvvm::Slot>();

  event_handler->Connect<mvvm::DataChangedEvent>(this, &AbstractViewModelController::OnEvent,
                                                 m_slot.get());
  event_handler->Connect<mvvm::AboutToInsertItemEvent>(this, &AbstractViewModelController::OnEvent,
                                                       m_slot.get());
  event_handler->Connect<mvvm::ItemInsertedEvent>(this, &AbstractViewModelController::OnEvent,
                                                  m_slot.get());
  event_handler->Connect<mvvm::AboutToRemoveItemEvent>(this, &AbstractViewModelController::OnEvent,
                                                       m_slot.get());
  event_handler->Connect<mvvm::ItemRemovedEvent>(this, &AbstractViewModelController::OnEvent,
                                                 m_slot.get());

  event_handler->Connect<mvvm::ModelAboutToBeResetEvent>(
      this, &AbstractViewModelController::OnEvent, m_slot.get());
  event_handler->Connect<mvvm::ModelResetEvent>(this, &AbstractViewModelController::OnEvent,
                                                m_slot.get());

  event_handler->Connect<mvvm::ModelAboutToBeDestroyedEvent>(
      this, &AbstractViewModelController::OnEvent, m_slot.get());
}

void AbstractViewModelController::Unsubscribe()
{
  m_slot.reset();
}

void AbstractViewModelController::OnEvent(const event_variant_t &event)
{
  std::visit(*this, event);
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

void AbstractViewModelController::OnDataChanged(const DataChangedEvent &event) {}

void AbstractViewModelController::OnModelAboutToBeReset(SessionModelInterface *model) {}

void AbstractViewModelController::OnModelReset(SessionModelInterface *model) {}

void AbstractViewModelController::OnModelAboutToBeDestroyed(SessionModelInterface *model) {}

void AbstractViewModelController::Init(SessionItem *) {}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

void AbstractViewModelController::operator()(const DataChangedEvent &event)
{
  OnDataChanged(event);
}

void AbstractViewModelController::operator()(const AboutToInsertItemEvent &event)
{
  OnAboutToInsertItem(event.m_parent, event.m_tag_index);
}

void AbstractViewModelController::operator()(const ItemInsertedEvent &event)
{
  OnItemInserted(event.m_parent, event.m_tag_index);
}

void AbstractViewModelController::operator()(const AboutToRemoveItemEvent &event)
{
  OnAboutToRemoveItem(event.m_parent, event.m_tag_index);
}

void AbstractViewModelController::operator()(const ItemRemovedEvent &event)
{
  OnItemRemoved(event.m_parent, event.m_tag_index);
}

void AbstractViewModelController::operator()(const ModelAboutToBeResetEvent &event)
{
  OnModelAboutToBeReset(event.m_model);
}

void AbstractViewModelController::operator()(const ModelResetEvent &event)
{
  OnModelReset(event.m_model);
}

void AbstractViewModelController::operator()(const ModelAboutToBeDestroyedEvent &event)
{
  OnModelAboutToBeDestroyed(event.m_model);
}

}  // namespace mvvm
