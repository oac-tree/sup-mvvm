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

#include "mvvm/signals/event_types.h"

namespace mvvm
{

// ----------------------------------------------------------------------------
// DataChangedEvent
// ----------------------------------------------------------------------------

DataChangedEvent::DataChangedEvent(int data_role, SessionItem* item)
    : m_data_role(data_role), m_item(item)
{
}

bool DataChangedEvent::operator==(const DataChangedEvent& other) const
{
  return m_data_role == other.m_data_role && m_item == other.m_item;
}

bool DataChangedEvent::operator!=(const DataChangedEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

AboutToInsertItemEvent::AboutToInsertItemEvent(SessionItem* parent, const TagIndex& tag_index)
    : m_parent(parent), m_tag_index(tag_index)
{
}

bool AboutToInsertItemEvent::operator==(const AboutToInsertItemEvent& other) const
{
  return m_parent == other.m_parent && m_tag_index == other.m_tag_index;
}

bool AboutToInsertItemEvent::operator!=(const AboutToInsertItemEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

ItemInsertedEvent::ItemInsertedEvent(SessionItem* item, const TagIndex& tag_index)
    : m_parent(item), m_tag_index(tag_index)
{
}

bool ItemInsertedEvent::operator==(const ItemInsertedEvent& other) const
{
  return m_parent == other.m_parent && m_tag_index == other.m_tag_index;
}

bool ItemInsertedEvent::operator!=(const ItemInsertedEvent& other) const
{
  return !(*this == other);
}

}  // namespace mvvm::experimental
