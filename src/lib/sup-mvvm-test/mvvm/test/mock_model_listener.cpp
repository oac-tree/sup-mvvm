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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mock_model_listener.h"

namespace mvvm::test
{

MockModelListener::MockModelListener(const ISessionModel *model) : ModelListener(model)
{
  Connect<DataChangedEvent>(this, &MockModelListener::OnDataChangedEvent);

  Connect<AboutToInsertItemEvent>(this, &MockModelListener::OnAboutToInsertItemEvent);
  Connect<ItemInsertedEvent>(this, &MockModelListener::OnItemInsertedEvent);
  Connect<AboutToRemoveItemEvent>(this, &MockModelListener::OnAboutToRemoveItemEvent);
  Connect<ItemRemovedEvent>(this, &MockModelListener::OnItemRemovedEvent);

  Connect<ModelAboutToBeResetEvent>(this, &MockModelListener::OnModelAboutToBeResetEvent);
  Connect<ModelResetEvent>(this, &MockModelListener::OnModelResetEvent);

  Connect<ModelAboutToBeDestroyedEvent>(this, &MockModelListener::OnModelAboutToBeDestroyedEvent);
}

}  // namespace mvvm::test
