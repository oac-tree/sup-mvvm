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

#ifndef MVVM_UTILS_THREADSAFE_QUEUE_H_
#define MVVM_UTILS_THREADSAFE_QUEUE_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>

//! @file threadsafe_queue.h
//! @brief Thread-safe queue borrowed from Anthony Williams, C++ Concurrency in Action, Second
//! edition.

namespace mvvm
{

struct empty_queue : public std::exception
{
  const char* what() const noexcept { return "Empty queue"; }
};

//! @class threadsafe_queue
//! @brief Thread-safe queue borrowed from Anthony Williams, C++ Concurrency in Action, Second
//! edition, and extended with `stop` methods.

template <typename T>
class threadsafe_queue
{
public:
  threadsafe_queue() {}

  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(new_value));
    m_data_condition.notify_one();
  }

  void wait_and_pop(T& value)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_queue.empty() || !m_is_in_operation; });
    if (m_queue.empty())
    {
      throw empty_queue();
    }

    value = std::move(m_queue.front());
    m_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_queue.empty() || !m_is_in_operation; });
    if (m_queue.empty())
    {
      throw empty_queue();
    }

    std::shared_ptr<T> res(std::make_shared<T>(std::move(m_queue.front())));
    m_queue.pop();
    return res;
  }

  bool try_pop(T& value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty())
    {
      return false;
    }
    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  std::shared_ptr<T> try_pop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty())
    {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res(std::make_shared<T>(std::move(m_queue.front())));
    m_queue.pop();
    return res;
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
  }

  void stop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_is_in_operation = false;
    m_data_condition.notify_all();
  }

private:
  mutable std::mutex m_mutex;
  std::queue<T> m_queue;
  std::condition_variable m_data_condition;
  //!< Variables that indicates that the queue is "up and running". When set to false, all possible
  //!< threads waiting at wait_and_pop will be get an exception.
  std::atomic<bool> m_is_in_operation{true};
};

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_QUEUE_H_
