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

#include "item_event_visitor.h"

namespace mvvm
{

void ItemEventVisitor::operator()(const DataChangedEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()(const PropertyChangedEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()(const AboutToInsertItemEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()(const ItemInsertedEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()(const AboutToRemoveItemEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()(const ItemRemovedEvent& event)
{
  m_source = event.item;
}

void ItemEventVisitor::operator()([[maybe_unused]] const ModelAboutToBeResetEvent& event)
{
  m_source = nullptr;
}

void ItemEventVisitor::operator()([[maybe_unused]] const ModelResetEvent& event)
{
  m_source = nullptr;
}

void ItemEventVisitor::operator()([[maybe_unused]] const ModelAboutToBeDestroyedEvent& event)
{
  m_source = nullptr;
}

}  // namespace mvvm
