/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_listener.h>

namespace mvvm
{

AbstractViewModelController::AbstractViewModelController() = default;
AbstractViewModelController::~AbstractViewModelController() = default;

void AbstractViewModelController::SetModel(ISessionModel *model)
{
  SetRootItem(model ? model->GetRootItem() : nullptr);
}

const ISessionModel *AbstractViewModelController::GetModel() const
{
  return m_listener ? m_listener->GetModel() : nullptr;
}

void AbstractViewModelController::OnModelEvent(const AboutToInsertItemEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const ItemInsertedEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const ItemRemovedEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const DataChangedEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeResetEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const ModelResetEvent &event)
{
  (void)event;
}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeDestroyedEvent &event)
{
  (void)event;
}

void AbstractViewModelController::SetRootItem(SessionItem *root_item)
{
  // It will subscribe to model notifications, and regenerate view model according to child/row
  // strategies. If an item is nullptr, will reset the view model, and unsubscribe from all
  // SessionModel notifications. If same item was already set, will do nothing.

  if (!root_item)
  {
    Unsubscribe();
    SetRootItemImpl(nullptr);
    return;
  }

  if (root_item->GetModel() == nullptr)
  {
    throw RuntimeException("Root item should belong to the model");
  }

  if (root_item->GetModel() != GetModel())
  {
    Unsubscribe();
    Subscribe(root_item->GetModel());
  }

  if (GetRootItem() != root_item)
  {
    SetRootItemImpl(root_item);
  }
}

int AbstractViewModelController::GetColumnCount() const
{
  return 0;
}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

void AbstractViewModelController::SubscribeAll(ISessionModel *model)
{
  m_listener = std::make_unique<mvvm::ModelListener>(model);

  m_listener->Connect<mvvm::DataChangedEvent>(this, &AbstractViewModelController::OnModelEvent);

  m_listener->Connect<mvvm::AboutToInsertItemEvent>(this,
                                                    &AbstractViewModelController::OnModelEvent);
  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &AbstractViewModelController::OnModelEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(this,
                                                    &AbstractViewModelController::OnModelEvent);
  m_listener->Connect<mvvm::ItemRemovedEvent>(this, &AbstractViewModelController::OnModelEvent);

  m_listener->Connect<mvvm::ModelAboutToBeResetEvent>(this,
                                                      &AbstractViewModelController::OnModelEvent);
  m_listener->Connect<mvvm::ModelResetEvent>(this, &AbstractViewModelController::OnModelEvent);

  m_listener->Connect<mvvm::ModelAboutToBeDestroyedEvent>(
      this, &AbstractViewModelController::OnModelEvent);
}

void AbstractViewModelController::Subscribe(ISessionModel *model)
{
  SubscribeImpl(model);
}

void AbstractViewModelController::SubscribeImpl(ISessionModel *model)
{
  SubscribeAll(model);
}

void AbstractViewModelController::Unsubscribe()
{
  if (m_listener)
  {
    UnsubscribeImpl();
    m_listener.reset();
  }
}

void AbstractViewModelController::UnsubscribeImpl() {}

}  // namespace mvvm
