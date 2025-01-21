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

namespace
{
/**
 * @brief An integer constant to specify the end of the container.
 */
const std::int32_t kAppendIndex = -1;

/**
 * @brief An integer constant used to mark TagIndex as invalid state.
 * @see TagIndex::IsValid()
 */
const std::int32_t kInvalidIndex = -2;
}  // namespace

namespace mvvm
{

TagIndex::TagIndex(const std::string& name, int32_t item_index) : m_tag(name), m_index(item_index)
{
}

TagIndex::TagIndex(const char* name, int32_t item_index) : m_tag(name), m_index(item_index) {}

std::string TagIndex::GetTag() const
{
  return m_tag;
}

int32_t TagIndex::GetIndex() const
{
  return m_index;
}

TagIndex TagIndex::Next() const
{
  return {m_tag, m_index + 1};
}

TagIndex TagIndex::Prev() const
{
  return {m_tag, m_index - 1};
}

bool TagIndex::IsValid() const
{
  return m_index != kInvalidIndex;
}

TagIndex TagIndex::Append(const std::string& tag_name)
{
  return {tag_name, kAppendIndex};
}

TagIndex TagIndex::First(const std::string& tag_name)
{
  return {tag_name, 0};
}

TagIndex TagIndex::Default(std::int32_t item_index)
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
