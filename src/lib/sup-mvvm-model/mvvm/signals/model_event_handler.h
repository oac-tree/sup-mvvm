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

#ifndef MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_
#define MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_

#include <mvvm/model_export.h>
#include <mvvm/signals/event_handler.h>
#include <mvvm/signals/event_types.h>

namespace mvvm
{

/**
 * @brief The ModelEventHandler class provides notification for all subscribers when some event
 * happened with SessionModel.
 */
class MVVM_MODEL_EXPORT ModelEventHandler : public EventHandler<event_variant_t>
{
public:
  ModelEventHandler();
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_EVENT_HANDLER_H_
