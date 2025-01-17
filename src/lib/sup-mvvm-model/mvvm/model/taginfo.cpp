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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "taginfo.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/container_utils.h>

#include <sstream>

namespace mvvm
{
TagInfo::TagInfo() : m_min(0), m_max(-1) {}

TagInfo::TagInfo(std::string name, int min, int max, std::vector<std::string> item_types)
    : m_name(std::move(name)), m_min(min), m_max(max), m_item_types(std::move(item_types))
{
  if (m_min < 0 || (m_min > m_max && m_max >= 0) || m_name.empty())
  {
    std::ostringstream ostr;
    ostr << "Invalid constructor parameters"
         << " " << m_name << " " << m_min << " " << m_max;
    throw RuntimeException(ostr.str());
  }
}

TagInfo TagInfo::CreateUniversalTag(std::string name, std::vector<std::string> item_types)
{
  return TagInfo(std::move(name), 0, -1, std::move(item_types));
}

TagInfo TagInfo::CreatePropertyTag(std::string name, std::string item_type)
{
  return TagInfo(std::move(name), 1, 1, {std::move(item_type)});
}

std::string TagInfo::GetName() const
{
  return m_name;
}

int TagInfo::GetMin() const
{
  return m_min;
}

int TagInfo::GetMax() const
{
  return m_max;
}

std::vector<std::string> TagInfo::GetItemTypes() const
{
  return m_item_types;
}

bool TagInfo::IsValidType(const std::string& item_type) const
{
  // if vector is empty, every type is considered as matching
  return m_item_types.empty() ? true : utils::Contains(m_item_types, item_type);
}

bool TagInfo::operator==(const TagInfo& other) const
{
  return m_name == other.m_name && m_min == other.m_min && m_max == other.m_max
         && m_item_types == other.m_item_types;
}

bool TagInfo::operator!=(const TagInfo& other) const
{
  return !(*this == other);
}

}  // namespace mvvm
