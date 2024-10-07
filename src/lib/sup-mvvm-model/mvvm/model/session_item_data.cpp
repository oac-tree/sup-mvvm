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

#include "session_item_data.h"

#include <mvvm/core/exceptions.h>

#include <algorithm>
#include <sstream>

namespace mvvm
{

std::vector<int> SessionItemData::GetRoles() const
{
  std::vector<int> result;
  std::transform(m_values.begin(), m_values.end(), std::back_inserter(result),
                 [](const auto& role_data) { return role_data.first; });
  return result;
}

variant_t SessionItemData::Data(int role) const
{
  for (const auto& value : m_values)
  {
    if (value.first == role)
    {
      return value.second;
    }
  }
  return {};
}

bool SessionItemData::SetData(const variant_t& value, int role)
{
  AssureCompatibility(value, role);

  for (auto it = m_values.begin(); it != m_values.end(); ++it)
  {
    if (it->first == role)
    {
      if (utils::IsValid(value))
      {
        if (it->second == value)
        {
          return false;
        }
        it->second = value;
      }
      else
      {
        m_values.erase(it);
      }
      return true;
    }
  }
  m_values.emplace_back(role, value);
  return true;
}

bool SessionItemData::HasData(int role) const
{
  auto has_role = [role](const auto& role_data) { return role_data.first == role; };
  return std::find_if(m_values.begin(), m_values.end(), has_role) != m_values.end();
}

void SessionItemData::AssureCompatibility(const variant_t& variant, int role) const
{
  if (!utils::AreCompatible(Data(role), variant))
  {
    std::ostringstream ostr;
    ostr << "Error in SessionItemData: variant types mismatch. "
         << "Old variant type [" << utils::TypeName(Data(role)) << "], "
         << "old value [" << utils::ValueToString(Data(role)) << "], "
         << "new variant type [" << utils::TypeName(variant) << "], "
         << "new value [" << utils::ValueToString(variant) << "], "
         << "role [" << role << "].\n";
    throw RuntimeException(ostr.str());
  }
}

SessionItemData::const_iterator SessionItemData::begin() const
{
  return m_values.begin();
}

SessionItemData::const_iterator SessionItemData::end() const
{
  return m_values.end();
}

}  // namespace mvvm
