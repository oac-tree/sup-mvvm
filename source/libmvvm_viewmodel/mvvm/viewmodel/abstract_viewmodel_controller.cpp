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

#include <mvvm/signals/model_listener.h>

namespace mvvm
{

struct AbstractViewModelController::AbstractViewModelControllerImpl
{
  void Subscribe(SessionModelInterface *model, AbstractViewModelController *self)
  {
    if (!model)
    {
      throw RuntimeException("Subscriber is not initialised");
    }

    m_listener = std::make_unique<mvvm::ModelListener<SessionModelInterface>>(model);

    m_listener->Connect<mvvm::DataChangedEvent>(self, &AbstractViewModelController::OnDataChanged);

    m_listener->Connect<mvvm::AboutToInsertItemEvent>(
        self, &AbstractViewModelController::OnAboutToInsertItem);
    m_listener->Connect<mvvm::ItemInsertedEvent>(self,
                                                 &AbstractViewModelController::OnItemInserted);
    m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
        self, &AbstractViewModelController::OnAboutToRemoveItem);
    m_listener->Connect<mvvm::ItemRemovedEvent>(self, &AbstractViewModelController::OnItemRemoved);

    m_listener->Connect<mvvm::ModelAboutToBeResetEvent>(
        self, &AbstractViewModelController::OnModelAboutToBeReset);
    m_listener->Connect<mvvm::ModelResetEvent>(self, &AbstractViewModelController::OnModelReset);

    m_listener->Connect<mvvm::ModelAboutToBeDestroyedEvent>(
        self, &AbstractViewModelController::OnModelAboutToBeDestroyed);
  }

  std::unique_ptr<ModelListener<SessionModelInterface>> m_listener;
};

AbstractViewModelController::~AbstractViewModelController() = default;

AbstractViewModelController::AbstractViewModelController()
    : p_impl(std::make_unique<AbstractViewModelControllerImpl>())
{
}

void AbstractViewModelController::Subscribe(SessionModelInterface *model)
{
  p_impl->Subscribe(model, this);
}

void AbstractViewModelController::Unsubscribe()
{
  p_impl->m_listener.reset();
}

void AbstractViewModelController::OnAboutToInsertItem(const AboutToInsertItemEvent &event) {}

void AbstractViewModelController::OnItemInserted(const ItemInsertedEvent &event) {}

void AbstractViewModelController::OnAboutToRemoveItem(const AboutToRemoveItemEvent &event) {}

void AbstractViewModelController::OnItemRemoved(const ItemRemovedEvent &event) {}

void AbstractViewModelController::OnDataChanged(const DataChangedEvent &event) {}

void AbstractViewModelController::OnModelAboutToBeReset(const ModelAboutToBeResetEvent &event) {}

void AbstractViewModelController::OnModelReset(const ModelResetEvent &event) {}

void AbstractViewModelController::OnModelAboutToBeDestroyed(
    const ModelAboutToBeDestroyedEvent &event)
{
}

void AbstractViewModelController::Init(SessionItem *) {}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

}  // namespace mvvm
