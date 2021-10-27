/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_EXPERIMENTAL_ANYVALUE_H
#define MVVM_EXPERIMENTAL_ANYVALUE_H

#include "mvvm/model_export.h"

#include <initializer_list>
#include <map>
#include <string>
#include <variant>

namespace Experimental
{
class AnyValue
{
public:
  using data_t = std::variant<std::monostate, int, double, std::string, AnyValue>;
  using pair_t = std::pair<std::string, data_t>;

  AnyValue() = default;
  AnyValue(const std::string& name, const data_t& data);
  AnyValue(std::initializer_list<pair_t> pars);

  void Add(const std::string& name, const data_t& data);

  data_t& operator[](const std::string& name);

  template <typename T = data_t>
  T Data(const std::string& name) const;

  bool operator==(const AnyValue& other) const;

private:
  std::map<std::string, data_t> m_data;
};

template <typename T>
inline T AnyValue::Data(const std::string& name) const
{
  if constexpr (std::is_same_v<T, data_t>)
    return m_data.find(name)->second;  // if data_t is required, simply return it
  else
    return std::get<T>(m_data.find(name)->second);
}

}  // namespace Experimental

#endif  // MVVM_EXPERIMENTAL_ANYVALUE_H
