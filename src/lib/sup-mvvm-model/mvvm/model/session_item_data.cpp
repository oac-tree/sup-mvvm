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
  auto iter = m_values.find(role);
  return iter == m_values.end() ? variant_t() : iter->second;
}

bool SessionItemData::SetData(const variant_t& value, int role)
{
  auto iter = m_values.find(role);

  // if the role doesn't exist yet, the value will be set for this role
  if (iter == m_values.end())
  {
    if (utils::IsValid(value))
    {
      m_values.insert(iter, {role, value});
      return true;
    }
    return false;  // invalid value is ignored
  }

  // if the role already exists, underlying types should coincide for old and new values
  const auto& old_value = iter->second;
  AssureCompatibility(old_value, value, role);

  // if new value is valid, it will replace old value
  if (utils::IsValid(value))
  {
    if (old_value == value)
    {
      return false;  // same value is ignored
    }
    iter->second = value;
    return true;
  }

  // if new value is invalid, it will erase old value and role
  m_values.erase(iter);
  return true;
}

bool SessionItemData::HasData(int role) const
{
  return m_values.find(role) != m_values.end();
}

void SessionItemData::AssureCompatibility(const variant_t& old_value, const variant_t& new_value,
                                          int role) const
{
  if (!utils::AreCompatible(old_value, new_value))
  {
    std::ostringstream ostr;
    ostr << "Error in SessionItemData: variant types mismatch. "
         << "Old variant type [" << utils::TypeName(old_value) << "], "
         << "old value [" << utils::ValueToString(old_value) << "], "
         << "new variant type [" << utils::TypeName(new_value) << "], "
         << "new value [" << utils::ValueToString(new_value) << "], "
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
