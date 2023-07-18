/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/model_listener.h>

namespace mvvm
{

AbstractViewModelController::AbstractViewModelController() = default;
AbstractViewModelController::~AbstractViewModelController() = default;

void AbstractViewModelController::SetModel(SessionModelInterface *model)
{
  if (!model)
  {
    Unsubscribe();
    SetRootItemImpl(nullptr);
    return;
  }

  if (GetModel() != model)
  {
    Subscribe(model);
  }

  if (GetRootItem() != model->GetRootItem())
  {
    SetRootItemImpl(model->GetRootItem());
  }
}

const SessionModelInterface *AbstractViewModelController::GetModel() const
{
  return m_listener ? m_listener->GetModel() : nullptr;
}

void AbstractViewModelController::OnModelEvent(const AboutToInsertItemEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ItemInsertedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ItemRemovedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const DataChangedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeResetEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelResetEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeDestroyedEvent &event) {}

void AbstractViewModelController::SetRootItem(SessionItem *root_item)
{
  if (!root_item)
  {
    Unsubscribe();
    SetRootItemImpl(nullptr);
    return;
  }

  if (root_item->GetModel() != GetModel())
  {
    Subscribe(root_item->GetModel());
  }

  if (GetRootItem() != root_item)
  {
    SetRootItemImpl(root_item);
  }
}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

void AbstractViewModelController::SubscribeAll(SessionModelInterface *model)
{
  if (!model)
  {
    throw RuntimeException("Subscriber is not initialised");
  }

  m_listener = std::make_unique<mvvm::ModelListener<SessionModelInterface>>(model);

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

void AbstractViewModelController::Subscribe(SessionModelInterface *model)
{
  SubscribeAll(model);
}

void AbstractViewModelController::Unsubscribe()
{
  m_listener.reset();
}

}  // namespace mvvm
