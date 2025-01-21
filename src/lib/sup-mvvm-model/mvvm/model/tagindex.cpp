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

#include "tagindex.h"

#include <limits>

namespace
{
/**
 * @brief An integer constant to specify the end of the container.
 */
const std::size_t kAppendIndex = std::numeric_limits<std::size_t>::max();

/**
 * @brief An integer constant used to mark TagIndex as invalid state.
 * @see TagIndex::IsValid()
 */
const std::size_t kInvalidIndex = std::numeric_limits<std::size_t>::max() - 1;
}  // namespace

namespace mvvm
{

TagIndex::TagIndex(const std::string& name, std::size_t index) : m_tag(name), m_index(index) {}

TagIndex::TagIndex(const char* name, std::size_t index) : m_tag(name), m_index(index) {}

std::string TagIndex::GetTag() const
{
  return m_tag;
}

std::size_t TagIndex::GetIndex() const
{
  return m_index;
}

TagIndex TagIndex::Next() const
{
  if (m_index == std::numeric_limits<std::size_t>::max())
  {
    return TagIndex::Invalid();
  }

  return {m_tag, m_index + 1};
}

TagIndex TagIndex::Prev() const
{
  if (m_index == std::numeric_limits<std::size_t>::lowest())
  {
    return TagIndex::Invalid();
  }
  return {m_tag, m_index - 1};
}

bool TagIndex::IsValid() const
{
  return m_index != kInvalidIndex;
}

bool TagIndex::IsAppend() const
{
  return m_index == kAppendIndex;
}

TagIndex TagIndex::Append(const std::string& tag_name)
{
  return {tag_name, kAppendIndex};
}

TagIndex TagIndex::First(const std::string& tag_name)
{
  return {tag_name, 0};
}

TagIndex TagIndex::Default(std::uint32_t item_index)
{
  return {kDefaultTag, item_index};
}

TagIndex TagIndex::Invalid()
{
  return {kDefaultTag, kInvalidIndex};
}

bool TagIndex::operator==(const TagIndex& other) const
{
  return m_index == other.m_index && m_tag == other.m_tag;
}

bool TagIndex::operator!=(const TagIndex& other) const
{
  return !(*this == other);
}

}  // namespace mvvm
