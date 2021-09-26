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

#include "mvvm/experimental/anyvalue.h"

#include <vector>

namespace Experimental
{
AnyValue::AnyValue(const std::string& name, const data_t& data)
{
  Add(name, data);
}

AnyValue::AnyValue(std::initializer_list<pair_t> pars)
{
  for (const auto& p : pars)
  {
    m_data.insert(p);
  }
}

void AnyValue::Add(const std::string& name, const data_t& data)
{
  m_data[name] = data;
}

AnyValue::data_t& AnyValue::operator[](const std::string& name)
{
  return m_data[name];
}

bool AnyValue::operator==(const AnyValue& other) const
{
  // FIXME make real comparison
  return m_data.size() == other.m_data.size();
}

}  // namespace Experimental
