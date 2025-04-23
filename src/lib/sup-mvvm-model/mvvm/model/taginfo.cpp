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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "taginfo.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/container_utils.h>

#include <limits>

namespace mvvm
{

TagInfo::TagInfo(std::string name, const std::optional<std::size_t>& min,
                 const std::optional<std::size_t>& max, std::vector<std::string> item_types)
    : m_name(std::move(name)), m_min(min), m_max(max), m_item_types(std::move(item_types))
{
  if (m_name.empty())
  {
    throw RuntimeException("Tag name can't be empty");
  }

  if (m_min.has_value() && m_max.has_value() && m_min > m_max)
  {
    throw RuntimeException("TagInfo can't have min > max");
  }
}

TagInfo TagInfo::CreateUniversalTag(std::string name, std::vector<std::string> item_types)
{
  return TagInfo(std::move(name), {}, {}, std::move(item_types));
}

TagInfo TagInfo::CreatePropertyTag(std::string name, std::string item_type)
{
  return TagInfo(std::move(name), 1U, 1U, {std::move(item_type)});
}

std::string TagInfo::GetName() const
{
  return m_name;
}

bool TagInfo::HasMin() const
{
  return m_min.has_value();
}

std::size_t TagInfo::GetMin() const
{
  return m_min.value_or(0U);
}

bool TagInfo::HasMax() const
{
  return m_max.has_value();
}

std::size_t TagInfo::GetMax() const
{
  return m_max.value_or(std::numeric_limits<std::size_t>::max());
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
