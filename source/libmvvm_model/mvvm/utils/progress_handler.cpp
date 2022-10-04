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

#include "mvvm/utils/progress_handler.h"

namespace mvvm
{

ProgressHandler::ProgressHandler(ProgressHandler::callback_t callback, size_t max_ticks_count)
    : runner_callback(std::move(callback)), max_ticks_count(max_ticks_count)
{
}

void ProgressHandler::subscribe(ProgressHandler::callback_t callback)
{
  runner_callback = std::move(callback);
}

//! Sets expected ticks count, representing progress of a computation.

void ProgressHandler::setMaxTicksCount(size_t value)
{
  reset();
  max_ticks_count = value;
}

bool ProgressHandler::has_interrupt_request() const
{
  return interrupt_request;
}

//! Increment number of completed computation steps. Performs callback to inform
//! subscriber about current progress (in percents) and retrieves interrupt request flag.

void ProgressHandler::setCompletedTicks(size_t value)
{
  std::unique_lock<std::mutex> lock(mutex);
  completed_ticks += value;
  if (completed_ticks > max_ticks_count)
    max_ticks_count = completed_ticks + 1;
  int percentage_done = static_cast<int>(100.0 * completed_ticks / max_ticks_count);
  interrupt_request = runner_callback ? runner_callback(percentage_done) : interrupt_request;
}

//! Resets progress.

void ProgressHandler::reset()
{
  interrupt_request = false;
  completed_ticks = 0;
}

}  // namespace mvvm
