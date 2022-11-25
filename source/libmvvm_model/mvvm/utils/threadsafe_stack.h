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

#ifndef MVVM_UTILS_THREADSAFE_STACK_H_
#define MVVM_UTILS_THREADSAFE_STACK_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <thread>

//! @file threadsafe_stack.h
//! @brief Thread-safe stack borrowed from Anthony Williams, C++ Concurrency in Action, Second
//! edition.

namespace mvvm
{

struct empty_stack : public std::exception
{
  const char* what() const noexcept { return "Empty stack"; }
};

//! @class threadsafe_stack
//! @brief Thread-safe stack borrowed from Anthony Williams, C++ Concurrency in Action, Second
//! edition, and extended with `update_top` and `stop` methods.

template <typename T>
class threadsafe_stack
{
public:
  threadsafe_stack() {}
  ~threadsafe_stack() { stop(); }

  threadsafe_stack(const threadsafe_stack& other)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data = other.data;
  }
  threadsafe_stack& operator=(const threadsafe_stack& other) = delete;

  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push(std::move(new_value));
    m_data_condition.notify_one();
  }

  //! Updates top value in a stack.

  void update_top(T new_value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_data.empty())
    {
      m_data.pop();
    }
    m_data.push(std::move(new_value));
    m_data_condition.notify_one();
  }

  void wait_and_pop(T& value)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_data.empty() || !m_is_in_operation; });
    if (m_data.empty())
    {
      throw empty_stack();
    }
    value = std::move(m_data.top());
    m_data.pop();
  }

  std::shared_ptr<T> wait_and_pop()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data_condition.wait(lock, [this] { return !m_data.empty() || !m_is_in_operation; });
    if (m_data.empty())
    {
      throw empty_stack();
    }
    std::shared_ptr<T> const result(std::make_shared<T>(std::move(m_data.top())));
    m_data.pop();
    return result;
  }

  bool try_pop(T& value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
      return false;
    }
    value = std::move(m_data.top());
    m_data.pop();
    return true;
  }

  std::shared_ptr<T> try_pop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> result(std::make_shared<T>(std::move(m_data.top())));
    m_data.pop();
    return result;
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.empty();
  }

  //! Terminates waiting in wait_and_pop methods.

  void stop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_is_in_operation = false;
    m_data_condition.notify_all();
  }

private:
  std::stack<T> m_data;
  mutable std::mutex m_mutex;
  std::condition_variable m_data_condition;
  //!< Variables that indicates that the stack is "up and running". When set to false, all possible
  //!< threads waiting at wait_and_pop will be get an exception.
  std::atomic<bool> m_is_in_operation{true};
};

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_STACK_H_
