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

bool operator==(const DataChangedEvent& lhs, const DataChangedEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.data_role == rhs.data_role);
}

bool operator!=(const DataChangedEvent& lhs, const DataChangedEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// PropertyChangedEvent
// ----------------------------------------------------------------------------

bool operator==(const PropertyChangedEvent& lhs, const PropertyChangedEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.name == rhs.name);
}

bool operator!=(const PropertyChangedEvent& lhs, const PropertyChangedEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

bool operator==(const AboutToInsertItemEvent& lhs, const AboutToInsertItemEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.tag_index == rhs.tag_index);
}

bool operator!=(const AboutToInsertItemEvent& lhs, const AboutToInsertItemEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// AboutToInsertItemEvent
// ----------------------------------------------------------------------------

bool operator==(const ItemInsertedEvent& lhs, const ItemInsertedEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.tag_index == rhs.tag_index);
}

bool operator!=(const ItemInsertedEvent& lhs, const ItemInsertedEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// AboutToRemoveItemEvent
// ----------------------------------------------------------------------------

bool operator==(const AboutToRemoveItemEvent& lhs, const AboutToRemoveItemEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.tag_index == rhs.tag_index);
}

bool operator!=(const AboutToRemoveItemEvent& lhs, const AboutToRemoveItemEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// ItemRemovedEvent
// ----------------------------------------------------------------------------

bool operator==(const ItemRemovedEvent& lhs, const ItemRemovedEvent& rhs)
{
  return (lhs.item == rhs.item) && (lhs.tag_index == rhs.tag_index);
}

bool operator!=(const ItemRemovedEvent& lhs, const ItemRemovedEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// ModelAboutToBeResetEvent
// ----------------------------------------------------------------------------

bool operator==(const ModelAboutToBeResetEvent& lhs, const ModelAboutToBeResetEvent& rhs)
{
  return lhs.model == rhs.model;
}

bool operator!=(const ModelAboutToBeResetEvent& lhs, const ModelAboutToBeResetEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// ModelResetEvent
// ----------------------------------------------------------------------------

bool operator==(const ModelResetEvent& lhs, const ModelResetEvent& rhs)
{
  return lhs.model == rhs.model;
}

bool operator!=(const ModelResetEvent& lhs, const ModelResetEvent& rhs)
{
  return !(lhs == rhs);
}

// ----------------------------------------------------------------------------
// ModelAboutToBeDestroyedEvent
// ----------------------------------------------------------------------------

bool operator==(const ModelAboutToBeDestroyedEvent& lhs, const ModelAboutToBeDestroyedEvent& rhs)
{
  return lhs.model == rhs.model;
}

bool operator!=(const ModelAboutToBeDestroyedEvent& lhs, const ModelAboutToBeDestroyedEvent& rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
