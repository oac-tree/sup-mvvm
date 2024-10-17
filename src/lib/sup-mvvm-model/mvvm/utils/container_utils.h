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

#ifndef MVVM_UTILS_CONTAINER_UTILS_H_
#define MVVM_UTILS_CONTAINER_UTILS_H_

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace mvvm::utils
{
template <class T>
struct is_unique_ptr : std::false_type
{
};

template <class T, class D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{
};

/**
 * @brief Returns index corresponding to the first occurrence of the item in the container (iterator
 * version).
 *
 * If the item doesn't exist, will return -1. Works with containers containing unique_ptr.
 */
template <typename It, typename T>
int IndexOfItem(It begin, It end, const T& item)
{
  It pos;
  if constexpr (is_unique_ptr<typename std::iterator_traits<It>::value_type>::value)
  {
    pos = find_if(begin, end, [&item](const auto& x) { return x.get() == item; });
  }
  else
  {
    pos = find(begin, end, item);
  }

  return pos == end ? -1 : static_cast<int>(std::distance(begin, pos));
}

/**
 * @brief Returns index corresponding to the first occurrence of the item in the container.
 *
 * If the item doesn't exist, will return -1. Works with containers containing unique_ptr.
 */
template <typename C, typename T>
int IndexOfItem(const C& container, const T& item)
{
  return IndexOfItem(container.begin(), container.end(), item);
}

/**
 * @brief Returns the copy of the container with all duplicated elements filtered out. The order is
 * preserved.
 */
template <typename C>
C UniqueWithOrder(const C& container)
{
  C result;

  using valueType = typename C::value_type;
  std::unordered_set<valueType> unique;

  std::copy_if(container.begin(), container.end(), std::back_inserter(result),
               [&unique](auto x) { return unique.insert(x).second; });

  return result;
}

/**
 * @brief Checks if the container holds the given element.
 */
template <typename A, typename B>
bool Contains(const A& container, const B& element)
{
  return std::find(container.begin(), container.end(), element) != container.end();
}

/**
 * @brief Cast items in the container to the given type and return the result to the user.
 *
 * The new container holds new types. If the item can't be casted it will be filtered out.
 */
template <typename T, typename C>
std::vector<T*> CastItems(const C& container)
{
  std::vector<T*> result;
  for (auto item : container)
  {
    if (auto casted = dynamic_cast<T*>(item); casted)
    {
      result.push_back(casted);
    }
  }
  return result;
}

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_CONTAINER_UTILS_H_
