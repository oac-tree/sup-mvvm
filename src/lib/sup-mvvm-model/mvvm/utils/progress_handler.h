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

#ifndef MVVM_UTILS_PROGRESS_HANDLER_H_
#define MVVM_UTILS_PROGRESS_HANDLER_H_

#include "mvvm/model_export.h"

#include <functional>
#include <mutex>

namespace mvvm
{

//! Maintain information about progress of a computation.
//! Initialized with callback function to report progress and retrieve interruption request status.

class MVVM_MODEL_EXPORT ProgressHandler
{
public:
  using callback_t = std::function<bool(size_t)>;

  ProgressHandler() = default;
  ProgressHandler(callback_t callback, size_t max_ticks_count);

  ProgressHandler(const ProgressHandler& other) = delete;
  ProgressHandler& operator=(const ProgressHandler& other) = delete;

  void Subscribe(callback_t callback);

  void SetMaxTicksCount(size_t value);

  bool HasInterruptRequest() const;

  void SetCompletedTicks(size_t value);

  void Reset();

private:
  std::mutex m_mutex;
  callback_t m_runner_callback;
  size_t m_max_ticks_count{0};
  size_t m_completed_ticks{0};
  bool m_interrupt_request{false};
};

}  // namespace mvvm

#endif  // MVVM_UTILS_PROGRESS_HANDLER_H_
