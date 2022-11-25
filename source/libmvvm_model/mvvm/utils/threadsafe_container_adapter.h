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

#ifndef MVVM_UTILS_THREADSAFE_CONTAINER_ADAPTER_H_
#define MVVM_UTILS_THREADSAFE_CONTAINER_ADAPTER_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace mvvm
{

struct empty_container_exception : public std::exception
{
  const char* what() const noexcept { return "Empty container"; }
};

//! @class threadsafe_container_adapter
//! @brief Threadsafe container adapter for stacks and queues.
//! Based on "Anthony Williams, C++ Concurrency in Action, Second edition".

template <typename ContainerT>
class threadsafe_container_adapter
{
public:
  using value_t = typename ContainerT::value_type;

  threadsafe_container_adapter() {}
  ~threadsafe_container_adapter() { stop(); }

  void push(value_t new_value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(new_value));
    m_data_condition.notify_one();
  }

  void wait_and_pop(value_t& value)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_queue.empty() || !m_is_in_operation; });
    if (m_queue.empty())
    {
      throw empty_container_exception();
    }

    value = std::move(m_queue.front());
    m_queue.pop();
  }

  std::shared_ptr<value_t> wait_and_pop()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_queue.empty() || !m_is_in_operation; });
    if (m_queue.empty())
    {
      throw empty_container_exception();
    }

    std::shared_ptr<value_t> res(std::make_shared<value_t>(std::move(m_queue.front())));
    m_queue.pop();
    return res;
  }

  bool try_pop(value_t& value)
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

  std::shared_ptr<value_t> try_pop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty())
    {
      return std::shared_ptr<value_t>();
    }
    std::shared_ptr<value_t> res(std::make_shared<value_t>(std::move(m_queue.front())));
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
  ContainerT m_queue;
  std::condition_variable m_data_condition;
  //!< Variables that indicates that the queue is "up and running". When set to false, all possible
  //!< threads waiting at wait_and_pop will get an exception.
  std::atomic<bool> m_is_in_operation{true};
};

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_QUEUE_H_
