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

#include "mvvm/signals/event_handler.h"

namespace mvvm::experimental
{

EventHandler::EventHandler()
{
  m_signals.Put<DataChangedEvent>(std::make_unique<signal_t>());
  m_signals.Put<AboutToInsertItemEvent>(std::make_unique<signal_t>());
  m_signals.Put<ItemInsertedEvent>(std::make_unique<signal_t>());
}

}  // namespace mvvm::experimental
