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

#include "mvvm/viewmodel/viewmodelcontroller.h"

ModelView::ViewModelController::~ViewModelController() {}

void ModelView::ViewModelController::OnAboutToInsertItem(SessionItem *parent,
                                                         const TagIndex &tag_index)
{
}

void ModelView::ViewModelController::OnItemInserted(SessionItem *parent, const TagIndex &tag_index)
{
}

void ModelView::ViewModelController::OnAboutToRemoveItem(SessionItem *parent,
                                                         const TagIndex &tag_index)
{
}

void ModelView::ViewModelController::OnItemRemoved(SessionItem *parent, const TagIndex &tag_index)
{
}

void ModelView::ViewModelController::OnDataChanged(SessionItem *item, int role) {}
