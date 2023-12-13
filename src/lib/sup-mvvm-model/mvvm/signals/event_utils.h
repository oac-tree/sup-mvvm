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

#ifndef MVVM_SIGNALS_EVENT_UTILS_H_
#define MVVM_SIGNALS_EVENT_UTILS_H_

//! @file
//! Utility functions for model events.

#include <mvvm/signals/event_types.h>

namespace mvvm
{

//! Returns an item which is the source of given event.
SessionItem* GetEventSource(const event_variant_t& event);

}  // namespace mvvm

#endif  // MVVM_SIGNALS_EVENT_UTILS_H_
