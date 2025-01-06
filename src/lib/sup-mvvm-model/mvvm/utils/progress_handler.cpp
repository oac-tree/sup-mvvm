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

#include "progress_handler.h"

namespace mvvm
{

ProgressHandler::ProgressHandler(ProgressHandler::callback_t callback, size_t max_ticks_count)
    : m_runner_callback(std::move(callback)), m_max_ticks_count(max_ticks_count)
{
}

void ProgressHandler::Subscribe(ProgressHandler::callback_t callback)
{
  m_runner_callback = std::move(callback);
}

//! Sets expected ticks count, representing progress of a computation.

void ProgressHandler::SetMaxTicksCount(size_t value)
{
  Reset();
  m_max_ticks_count = value;
}

bool ProgressHandler::HasInterruptRequest() const
{
  return m_interrupt_request;
}

//! Increment number of completed computation steps. Performs callback to inform
//! subscriber about current progress (in percents) and retrieves interrupt request flag.

void ProgressHandler::SetCompletedTicks(size_t value)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_completed_ticks += value;
  if (m_completed_ticks > m_max_ticks_count)
  {
    m_max_ticks_count = m_completed_ticks + 1;
  }
  size_t percentage_done =
      static_cast<size_t>(100.0 * m_completed_ticks / static_cast<double>(m_max_ticks_count));
  m_interrupt_request =
      m_runner_callback ? m_runner_callback(percentage_done) : m_interrupt_request;
}

//! Resets progress.

void ProgressHandler::Reset()
{
  m_interrupt_request = false;
  m_completed_ticks = 0;
}

}  // namespace mvvm
