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

#include "event_utils.h"

namespace
{

//! Helper class to visit event_variant_t and report the source of concrete event.
struct ItemEventVisitor
{
  mvvm::SessionItem* m_source{nullptr};

  void operator()(const mvvm::DataChangedEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::PropertyChangedEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::AboutToInsertItemEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::ItemInsertedEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::AboutToRemoveItemEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::ItemRemovedEvent& event) { m_source = event.m_item; }

  void operator()(const mvvm::ModelAboutToBeResetEvent& event)
  {
    // nothing to do
  }

  void operator()(const mvvm::ModelResetEvent& event)
  {
    // nothing to do
  }

  void operator()(const mvvm::ModelAboutToBeDestroyedEvent& event)
  {
    // nothing to do
  }
};

}  // namespace

namespace mvvm
{

SessionItem* GetEventSource(const event_variant_t& event)
{
  ItemEventVisitor event_visitor;
  std::visit(event_visitor, event);
  return event_visitor.m_source;
}

}  // namespace mvvm
