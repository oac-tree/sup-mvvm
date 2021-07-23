// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_UTILS_CONTAINERUTILS_H
#define MVVM_UTILS_CONTAINERUTILS_H

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

namespace ModelView::Utils
{
template <class T>
struct is_unique_ptr : std::false_type
{
};

template <class T, class D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{
};

//! Returns index corresponding to the first occurance of the item in the
//! container. If item doesn't exist, will return -1. Works with containers
//! containing unique_ptr.

template <typename It, typename T>
int IndexOfItem(It begin, It end, const T& item)
{
  It pos;
  if constexpr (is_unique_ptr<typename std::iterator_traits<It>::value_type>::value)
    pos = find_if(begin, end, [&item](const auto& x) { return x.get() == item; });
  else
    pos = find_if(begin, end, [&item](const auto& x) { return x == item; });

  return pos == end ? -1 : static_cast<int>(std::distance(begin, pos));
}

//! Returns index corresponding to the first occurance of the item in the
//! container. If item doesn't exist, will return -1. Works with containers
//! containing unique_ptr.

template <typename C, typename T>
int IndexOfItem(const C& container, const T& item)
{
  return IndexOfItem(container.begin(), container.end(), item);
}

//! Returns true if container contains a given element.

template <typename A, typename B>
bool Contains(const A& container, const B& element)
{
  return std::find(container.begin(), container.end(), element) != container.end();
}

}  // namespace ModelView::Utils

#endif // MVVM_UTILS_CONTAINERUTILS_H
