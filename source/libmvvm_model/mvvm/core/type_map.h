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

#ifndef MVVM_CORE_TYPE_MAP_H_
#define MVVM_CORE_TYPE_MAP_H_

//! @file type_map.h
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
  typedef std::map<int, ValueType> InternalMap;

public:
  typedef typename InternalMap::iterator iterator;
  typedef typename InternalMap::const_iterator const_iterator;
  typedef typename InternalMap::value_type value_type;

  const_iterator begin() const { return m_map.begin(); }
  const_iterator end() const { return m_map.end(); }
  iterator begin() { return m_map.begin(); }
  iterator end() { return m_map.end(); }

  // Finds the value associated with the type "Key" in the type map.
  template <class Key>
  iterator find()
  {
    return m_map.find(getTypeId<Key>());
  }

  // Same as above, const version
  template <class Key>
  const_iterator find() const
  {
    return m_map.find(getTypeId<Key>());
  }

  // Associates a value with the type "Key"
  template <class Key>
  void put(ValueType &&value)
  {
    m_map[getTypeId<Key>()] = std::forward<ValueType>(value);
  }

private:
  template <class Key>
  inline static int getTypeId()
  {
    static const int id = LastTypeId++;
    return id;
  }

  static std::atomic_int LastTypeId;
  InternalMap m_map;
};

template <class ValueType>
std::atomic_int TypeMap<ValueType>::LastTypeId(0);

}  // namespace mvvm

#endif  // MVVM_CORE_TYPE_MAP_H_
