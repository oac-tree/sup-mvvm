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

#include <mvvm/utils/threadsafe_container_adapter.h>

#include <stack>

namespace mvvm
{

//! Thread-safe stack.

template <typename T>
class threadsafe_stack : public threadsafe_container_adapter<std::stack<T>>
{
public:
  using base_t = threadsafe_container_adapter<std::stack<T>>;

  //! Updates top value in a stack.
  void update_top(T new_value)
  {
    std::lock_guard<std::mutex> lock(base_t::m_mutex);
    if (!base_t::m_data.empty())
    {
      base_t::m_data.pop();
    }
    base_t::m_data.push(std::move(new_value));
    base_t::m_data_condition.notify_one();
  }

protected:

};

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_STACK_H_
