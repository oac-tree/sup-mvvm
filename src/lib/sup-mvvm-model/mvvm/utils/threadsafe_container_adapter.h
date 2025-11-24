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

#ifndef MVVM_UTILS_THREADSAFE_CONTAINER_ADAPTER_H_
#define MVVM_UTILS_THREADSAFE_CONTAINER_ADAPTER_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>

namespace mvvm
{

struct empty_container_exception : public std::exception
{
  const char* what() const noexcept override { return "Empty container"; }
};

template <class T>
struct is_stack : std::false_type
{
};

template <class T>
struct is_stack<std::stack<T>> : std::true_type
{
};

//! @brief Threadsafe container adapter for stacks and queues.
//! Plays a role of the base class for threadsafe stack and queues. Based on "Anthony Williams, C++
//! Concurrency in Action, Second edition, Chapter 6".

template <typename ContainerT>
class threadsafe_container_adapter
{
public:
  using value_t = typename ContainerT::value_type;

  threadsafe_container_adapter() = default;
  ~threadsafe_container_adapter();

  threadsafe_container_adapter(const threadsafe_container_adapter& other);
  threadsafe_container_adapter& operator=(const threadsafe_container_adapter&) = delete;

  void push(value_t new_value);

  void wait_and_pop(value_t& value);

  std::shared_ptr<value_t> wait_and_pop();

  bool try_pop(value_t& value);

  std::shared_ptr<value_t> try_pop();

  bool empty() const;

  void stop();

  std::size_t size() const;

protected:
  typename ContainerT::reference GetTop();

  ContainerT m_data;
  mutable std::mutex m_mutex;
  std::condition_variable m_data_condition;

  //!< Variable that indicates that the queue is "up and running". When set to false, all possible
  //!< threads waiting at wait_and_pop will get an exception.
  bool m_is_in_operation{true};
};

template <typename ContainerT>
inline threadsafe_container_adapter<ContainerT>::~threadsafe_container_adapter()
{
  stop();
}

template <typename ContainerT>
inline threadsafe_container_adapter<ContainerT>::threadsafe_container_adapter(
    const threadsafe_container_adapter& other)
{
  std::scoped_lock lock(other.m_mutex);
  m_data = other.m_data;
}

template <typename ContainerT>
inline void threadsafe_container_adapter<ContainerT>::push(value_t new_value)
{
  std::scoped_lock lock(m_mutex);
  m_data.push(std::move(new_value));
  m_data_condition.notify_one();
}

template <typename ContainerT>
inline void threadsafe_container_adapter<ContainerT>::wait_and_pop(value_t& value)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_data_condition.wait(lock, [this] { return !m_data.empty() || !m_is_in_operation; });
  if (m_data.empty())
  {
    throw empty_container_exception();
  }

  value = std::move(GetTop());
  m_data.pop();
}

template <typename ContainerT>
inline bool threadsafe_container_adapter<ContainerT>::try_pop(value_t& value)
{
  std::scoped_lock lock(m_mutex);
  if (m_data.empty())
  {
    return false;
  }
  value = std::move(GetTop());
  m_data.pop();
  return true;
}

template <typename ContainerT>
inline bool threadsafe_container_adapter<ContainerT>::empty() const
{
  std::scoped_lock lock(m_mutex);
  return m_data.empty();
}

template <typename ContainerT>
inline void threadsafe_container_adapter<ContainerT>::stop()
{
  std::scoped_lock lock(m_mutex);
  m_is_in_operation = false;
  m_data_condition.notify_all();
}

template <typename ContainerT>
inline std::size_t threadsafe_container_adapter<ContainerT>::size() const
{
  std::scoped_lock lock(m_mutex);
  return m_data.size();
}

template <typename ContainerT>
inline typename ContainerT::reference threadsafe_container_adapter<ContainerT>::GetTop()
{
  if constexpr (is_stack<ContainerT>())
  {
    return m_data.top();
  }
  else
  {
    return m_data.front();
  }
}

template <typename ContainerT>
inline std::shared_ptr<typename threadsafe_container_adapter<ContainerT>::value_t>
threadsafe_container_adapter<ContainerT>::try_pop()
{
  std::scoped_lock lock(m_mutex);
  if (m_data.empty())
  {
    return std::shared_ptr<value_t>();
  }
  std::shared_ptr<value_t> result(std::make_shared<value_t>(std::move(GetTop())));
  m_data.pop();
  return result;
}

template <typename ContainerT>
inline std::shared_ptr<typename threadsafe_container_adapter<ContainerT>::value_t>
threadsafe_container_adapter<ContainerT>::wait_and_pop()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_data_condition.wait(lock, [this] { return !m_data.empty() || !m_is_in_operation; });
  if (m_data.empty())
  {
    throw empty_container_exception();
  }

  std::shared_ptr<value_t> result(std::make_shared<value_t>(std::move(GetTop())));
  m_data.pop();
  return result;
}

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_CONTAINER_ADAPTER_H_
