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

#include "item_connect_helper.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

namespace
{

/**
 * @brief The ItemEventVisitor is a helper class to visit event_variant_t and report the source of
 * concrete event.
 */
struct ItemEventVisitor
{
  mvvm::SessionItem* m_source{nullptr};

  void operator()(const mvvm::DataChangedEvent& event) { m_source = event.item; }

  void operator()(const mvvm::PropertyChangedEvent& event) { m_source = event.item; }

  void operator()(const mvvm::AboutToInsertItemEvent& event) { m_source = event.item; }

  void operator()(const mvvm::ItemInsertedEvent& event) { m_source = event.item; }

  void operator()(const mvvm::AboutToRemoveItemEvent& event) { m_source = event.item; }

  void operator()(const mvvm::ItemRemovedEvent& event) { m_source = event.item; }

  void operator()(const mvvm::ModelAboutToBeResetEvent& event)
  {
    (void)event;
    // nothing to do
  }

  void operator()(const mvvm::ModelResetEvent& event)
  {
    (void)event;
    // nothing to do
  }

  void operator()(const mvvm::ModelAboutToBeDestroyedEvent& event)
  {
    (void)event;
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

mvvm::ModelEventHandler* GetEventHandler(const mvvm::SessionItem* item)
{
  if (!item)
  {
    throw NullArgumentException("Error in ItemConnectUtils: uninitialised item");
  }

  if (!item->GetModel())
  {
    throw NullArgumentException("Error in ItemConnectUtils: item doesn't have a model");
  }

  if (auto event_handler = item->GetModel()->GetEventHandler(); event_handler)
  {
    return event_handler;
  }
  throw LogicErrorException("The model doesn't have signaling capabilities");
}

std::optional<PropertyChangedEvent> ConvertToPropertyChangedEvent(SessionItem* source,
                                                                  const event_variant_t& event)
{
  // DataChangedEvent happened with property item can be converted to PropertyChangedEvent of its
  // parent.
  auto concrete_event = std::get<DataChangedEvent>(event);
  if (source == concrete_event.item->GetParent())
  {
    return PropertyChangedEvent{source, source->TagIndexOfItem(concrete_event.item).GetTag()};
  }
  return {};
}

}  // namespace mvvm
