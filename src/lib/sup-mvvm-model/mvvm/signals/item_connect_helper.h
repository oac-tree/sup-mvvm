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

#ifndef MVVM_SIGNALS_ITEM_CONNECT_HELPER_H_
#define MVVM_SIGNALS_ITEM_CONNECT_HELPER_H_

//! @file
//! Utility functions for item connection.

#include <mvvm/signals/event_types.h>

#include <optional>

namespace mvvm
{

class ModelEventHandler;

/**
 * @brief Returns an item which is the source of given event.
 */
SessionItem* GetEventSource(const event_variant_t& event);

/**
 * @brief Returns event handler.
 */
ModelEventHandler* GetEventHandler(const mvvm::SessionItem* item);

/**
 * @brief Converts event to PropertyChangedEvent.
 *
 * It is assumed that the underlying event is DataChangedEvent which happened with one of the
 * property items.
 */
std::optional<PropertyChangedEvent> ConvertToPropertyChangedEvent(SessionItem* source,
                                                                  const event_variant_t& event);

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_CONNECT_HELPER_H_
