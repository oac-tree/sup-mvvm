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

#include "session_item_impl.h"

#include "i_session_model.h"
#include "session_item.h"
#include "session_item_data.h"
#include "tagged_items.h"

#include <mvvm/signals/signal_slot.h>

namespace mvvm
{

SessionItemImpl::SessionItemImpl(const std::string &item_type,
                                 std::unique_ptr<SessionItemData> data,
                                 std::unique_ptr<TaggedItems> tags)
    : m_item_type(item_type), m_item_data(std::move(data)), m_tagged_items(std::move(tags))
{
}

SessionItemImpl::~SessionItemImpl() = default;

std::string SessionItemImpl::GetType() const
{
  return m_item_type;
}

variant_t SessionItemImpl::Data(int32_t role)
{
  return GetItemData()->Data(role);
}

bool SessionItemImpl::SetData(const variant_t &value, int32_t role)
{
  return GetItemData()->SetData(value, role);
}

SessionItemData *SessionItemImpl::GetItemData()
{
  return m_item_data.get();
}

TaggedItems *SessionItemImpl::GetTaggedItems()
{
  return m_tagged_items.get();
}

SessionItem *SessionItemImpl::GetParent()
{
  return m_parent;
}

ISessionModel *SessionItemImpl::GetModel()
{
  return m_model;
}

void SessionItemImpl::SetModel(ISessionModel *model, SessionItem *my_owner)
{
  if (m_model)
  {
    m_model->CheckOut(my_owner);
    m_slot.reset();  // we do not want to receive notification after removal from the model
  }

  m_model = model;

  // First we assign the model to all children, and then we register item.
  for (auto child : GetTaggedItems()->GetAllItems())
  {
    child->SetModel(model);
  }
  if (m_model)
  {
    m_model->CheckIn(my_owner);
  }
}

Slot *SessionItemImpl::GetSlot()
{
  if (!m_slot)
  {
    m_slot = std::make_unique<Slot>();
  }

  return m_slot.get();
}

}  // namespace mvvm
