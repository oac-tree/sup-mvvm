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

#include "event_types.h"

namespace mvvm
{

// ----------------------------------------------------------------------------
// DataChangedEvent
// ----------------------------------------------------------------------------

bool DataChangedEvent::operator==(const DataChangedEvent& other) const
{
  return item == other.item && data_role == other.data_role;
}

bool DataChangedEvent::operator!=(const DataChangedEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// PropertyChangedEvent
// ----------------------------------------------------------------------------

bool PropertyChangedEvent::operator==(const PropertyChangedEvent& other) const
{
  return item == other.item && name == other.name;
}

bool PropertyChangedEvent::operator!=(const PropertyChangedEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

bool AboutToInsertItemEvent::operator==(const AboutToInsertItemEvent& other) const
{
  return item == other.item && tag_index == other.tag_index;
}

bool AboutToInsertItemEvent::operator!=(const AboutToInsertItemEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

bool ItemInsertedEvent::operator==(const ItemInsertedEvent& other) const
{
  return item == other.item && tag_index == other.tag_index;
}

bool ItemInsertedEvent::operator!=(const ItemInsertedEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// AboutToRemoveItemEvent
// ----------------------------------------------------------------------------

bool AboutToRemoveItemEvent::operator==(const AboutToRemoveItemEvent& other) const
{
  return item == other.item && tag_index == other.tag_index;
}

bool AboutToRemoveItemEvent::operator!=(const AboutToRemoveItemEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// ItemRemovedEvent
// ----------------------------------------------------------------------------

bool ItemRemovedEvent::operator==(const ItemRemovedEvent& other) const
{
  return item == other.item && tag_index == other.tag_index;
}

bool ItemRemovedEvent::operator!=(const ItemRemovedEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// ModelAboutToBeResetEvent
// ----------------------------------------------------------------------------

bool ModelAboutToBeResetEvent::operator==(const ModelAboutToBeResetEvent& other) const
{
  return model == other.model;
}

bool ModelAboutToBeResetEvent::operator!=(const ModelAboutToBeResetEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// ModelResetEvent
// ----------------------------------------------------------------------------

bool ModelResetEvent::operator==(const ModelResetEvent& other) const
{
  return model == other.model;
}

bool ModelResetEvent::operator!=(const ModelResetEvent& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// ModelAboutToBeDestroyedEvent
// ----------------------------------------------------------------------------

bool ModelAboutToBeDestroyedEvent::operator==(const ModelAboutToBeDestroyedEvent& other) const
{
  return model == other.model;
}

bool ModelAboutToBeDestroyedEvent::operator!=(const ModelAboutToBeDestroyedEvent& other) const
{
  return !(*this == other);
}

}  // namespace mvvm
