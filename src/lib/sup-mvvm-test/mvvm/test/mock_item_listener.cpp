/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mock_item_listener.h"

#include "mvvm/signals/event_types.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagindex.h>

namespace mvvm::test
{

void MockItemListener::Subscribe()
{
  Connect<mvvm::ItemInsertedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::DataChangedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::PropertyChangedEvent>(this, &MockItemListener::OnEvent);
}

void MockItemListenerV2::Subscribe()
{
  Connect<mvvm::ItemInsertedEvent>(this, &MockItemListenerV2::OnItemInsertedEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockItemListenerV2::OnAboutToRemoveItemEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockItemListenerV2::OnItemRemovedEvent);
  Connect<mvvm::DataChangedEvent>(this, &MockItemListenerV2::OnDataChangedEvent);
  Connect<mvvm::PropertyChangedEvent>(this, &MockItemListenerV2::OnPropertyChangedEvent);
}

}  // namespace mvvm::test
