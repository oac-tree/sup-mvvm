/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_CORE_TYPE_MAP_H_
#define MVVM_CORE_TYPE_MAP_H_

//! @file
//! Map of types to store any value against type.
//! Credits to https://gpfault.net/posts/mapping-types-to-values.txt.html

#include <atomic>
#include <map>

namespace mvvm
{

//! Map of types to values.

template <class ValueType>
class TypeMap
{
  using container_t = std::map<int, ValueType>;

public:
  using iterator = typename container_t::iterator;
  using const_iterator = typename container_t::const_iterator;
  using value_type = typename container_t::value_type;

  const_iterator begin() const { return m_map.begin(); }
  const_iterator end() const { return m_map.end(); }
  iterator begin() { return m_map.begin(); }
  iterator end() { return m_map.end(); }

  //! Finds the value associated with the type "Key" in the type map.
  template <class Key>
  iterator Find()
  {
    return m_map.find(GetTypeId<Key>());
  }

  //! Same as above, const version
  template <class Key>
  const_iterator Find() const
  {
    return m_map.find(GetTypeId<Key>());
  }

  // Associates a value with the type "Key"
  template <class Key>
  void Put(ValueType &&value)
  {
    m_map[GetTypeId<Key>()] = std::forward<ValueType>(value);
  }

  template <class Key>
  static int GetTypeId()
  {
    static const int id = m_last_type_id++;
    return id;
  }

private:
  static std::atomic_int m_last_type_id;
  container_t m_map;
};

template <class ValueType>
std::atomic_int TypeMap<ValueType>::m_last_type_id(0);

}  // namespace mvvm

#endif  // MVVM_CORE_TYPE_MAP_H_
