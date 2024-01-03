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

#include "item_connect_utils.h"

#include "model_event_handler.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm::connect
{

mvvm::ModelEventHandler *GetEventHandler(const mvvm::SessionItem *item)
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

std::optional<PropertyChangedEvent> ConvertToPropertyChangedEvent(SessionItem *source,
                                                                  const event_variant_t &event)
{
  // DataChangedEvent happened with property item can be converted to PropertyChangedEvent of its
  // parent.
  auto concrete_event = std::get<DataChangedEvent>(event);
  if (source == concrete_event.m_item->GetParent())
  {
    return PropertyChangedEvent{source, source->TagIndexOfItem(concrete_event.m_item).tag};
  }
  return {};
}

}  // namespace mvvm::connect
