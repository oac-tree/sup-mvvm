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

#include "mock_item_listener.h"

#include "mvvm/signals/event_types.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/tagindex.h>

namespace mvvm::test
{

MockItemListener::MockItemListener(SessionItem *item) : mvvm::ItemListener(item)
{
  Connect<mvvm::ItemInsertedEvent>(this, &MockItemListener::OnItemInsertedEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockItemListener::OnAboutToRemoveItemEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockItemListener::OnItemRemovedEvent);
  Connect<mvvm::DataChangedEvent>(this, &MockItemListener::OnDataChangedEvent);
  Connect<mvvm::PropertyChangedEvent>(this, &MockItemListener::OnPropertyChangedEvent);
}

}  // namespace mvvm::test
