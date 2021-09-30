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

namespace ModelView
{
QStringList AbstractViewModelController::GetHorizontalHeaderLabels() const
{
  return {};
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

const SessionItem *AbstractViewModelController::GetRootSessionItem() const
{
  return nullptr;
}

void AbstractViewModelController::Init() {}

}  // namespace ModelView
