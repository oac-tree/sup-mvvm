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

    m_listener->Connect<mvvm::DataChangedEvent>(self, &AbstractViewModelController::OnModelEvent);

    m_listener->Connect<mvvm::AboutToInsertItemEvent>(self,
                                                      &AbstractViewModelController::OnModelEvent);
    m_listener->Connect<mvvm::ItemInsertedEvent>(self, &AbstractViewModelController::OnModelEvent);
    m_listener->Connect<mvvm::AboutToRemoveItemEvent>(self,
                                                      &AbstractViewModelController::OnModelEvent);
    m_listener->Connect<mvvm::ItemRemovedEvent>(self, &AbstractViewModelController::OnModelEvent);

    m_listener->Connect<mvvm::ModelAboutToBeResetEvent>(self,
                                                        &AbstractViewModelController::OnModelEvent);
    m_listener->Connect<mvvm::ModelResetEvent>(self, &AbstractViewModelController::OnModelEvent);

    m_listener->Connect<mvvm::ModelAboutToBeDestroyedEvent>(
        self, &AbstractViewModelController::OnModelEvent);
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

void AbstractViewModelController::OnModelEvent(const AboutToInsertItemEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ItemInsertedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ItemRemovedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const DataChangedEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeResetEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelResetEvent &event) {}

void AbstractViewModelController::OnModelEvent(const ModelAboutToBeDestroyedEvent &event) {}

void AbstractViewModelController::Init(SessionItem *) {}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

}  // namespace mvvm
