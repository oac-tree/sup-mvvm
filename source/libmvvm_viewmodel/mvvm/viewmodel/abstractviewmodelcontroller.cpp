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

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"

#include "mvvm/interfaces/modeleventnotifierinterface.h"

namespace mvvm
{
AbstractViewModelController::~AbstractViewModelController()
{
  if (m_notifier)
  {
    // Tells notifier that we do not need notifications anymnore.
    m_notifier->Unsubscribe(this);
  }
}

void AbstractViewModelController::SetNotifier(ModelEventNotifierInterface *notifier)
{
  m_notifier = notifier;
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

void AbstractViewModelController::OnModelAboutToBeReset(SessionModel *model) {}

void AbstractViewModelController::OnModelReset(SessionModel *model) {}

void AbstractViewModelController::OnModelAboutToBeDestroyed(SessionModel *model) {}

void AbstractViewModelController::Init(SessionItem *) {}

QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
}

}  // namespace mvvm
