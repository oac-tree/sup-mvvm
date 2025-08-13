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

#include "combo_property.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_utils.h>

#include <sstream>

namespace
{

const std::string kSeparator = ";";
const std::string kMultipleSelectionLabel = "Multiple";
const std::string kNoneSelectionLabel = "None";
const std::string kValueKey = "values";
const std::string kSelectionKey = "selection";

}  // namespace

namespace mvvm
{

ComboProperty::ComboProperty() = default;

ComboProperty::ComboProperty(std::vector<std::string> values, const std::string& current_value)
    : m_values(std::move(values))
{
  if (m_values.empty())
  {
    return;
  }

  if (!current_value.empty())
  {
    SetValue(current_value);
  }
  else
  {
    SetCurrentIndex(0);
  }
}

// cppcheck-suppress unusedFunction
ComboProperty ComboProperty::CreateFrom(const std::vector<std::string>& values,
                                        const std::string& current_value)
{
  return ComboProperty(values, current_value);
}

std::string ComboProperty::GetValue() const
{
  return GetCurrentIndex() < 0 ? std::string()
                               : m_values.at(static_cast<size_t>(GetCurrentIndex()));
}

void ComboProperty::SetValue(const std::string& name)
{
  if (!utils::Contains(m_values, name))
  {
    throw RuntimeException("Error in ComboProperty::setValue(): combo doesn't contain value ["
                           + name + "]");
  }
  SetCurrentIndex(utils::IndexOfItem(m_values, name));
}

std::vector<std::string> ComboProperty::GetValues() const
{
  return m_values;
}

void ComboProperty::SetValues(const std::vector<std::string>& values)
{
  if (values.empty())
  {
    return;
  }

  auto current = GetValue();
  m_values = values;
  SetCurrentIndex(utils::Contains(m_values, current) ? utils::IndexOfItem(m_values, current) : 0);
}

std::vector<std::string> ComboProperty::GetToolTips() const
{
  return m_tooltips;
}

// cppcheck-suppress unusedFunction
void ComboProperty::SetToolTips(const std::vector<std::string>& tooltips)
{
  m_tooltips = tooltips;
}

std::int32_t ComboProperty::GetCurrentIndex() const
{
  return m_selected_indices.empty() ? -1 : m_selected_indices.at(0);
}

void ComboProperty::SetCurrentIndex(std::int32_t index)
{
  if (index < 0 || index >= static_cast<std::int32_t>(m_values.size()))
  {
    throw RuntimeException("Error in ComboProperty::setCurrentIndex(): invalid index");
  }
  m_selected_indices.clear();
  m_selected_indices.push_back(index);
}

std::vector<std::int32_t> ComboProperty::GetSelectedIndices() const
{
  return m_selected_indices;
}

void ComboProperty::SetSelectedIndices(const std::vector<std::int32_t>& selected_indices)
{
  m_selected_indices = selected_indices;
}

std::vector<std::string> ComboProperty::GetSelectedValues() const
{
  std::vector<std::string> result;
  result.reserve(m_selected_indices.size());
  for (auto index : m_selected_indices)
  {
    result.push_back(m_values.at(static_cast<size_t>(index)));
  }
  return result;
}

void ComboProperty::SetSelected(std::int32_t index, bool value)
{
  if (index < 0 || index >= static_cast<std::int32_t>(m_values.size()))
  {
    return;
  }

  auto pos = find(m_selected_indices.begin(), m_selected_indices.end(), index);
  if (value)
  {
    if (pos == m_selected_indices.end())
    {
      m_selected_indices.push_back(index);
    }
  }
  else
  {
    if (pos != m_selected_indices.end())
    {
      m_selected_indices.erase(pos);
    }
  }
  std::sort(m_selected_indices.begin(), m_selected_indices.end());
}

void ComboProperty::SetSelected(const std::string& name, bool value)
{
  SetSelected(utils::IndexOfItem(m_values, name), value);
}

std::string ComboProperty::GetLabel() const
{
  if (m_selected_indices.size() > 1)
  {
    return kMultipleSelectionLabel;
  }

  if (m_selected_indices.size() == 1)
  {
    return GetValue();
  }

  return kNoneSelectionLabel;
}

ComboProperty& ComboProperty::operator<<(const std::string& str)
{
  m_values.push_back(str);
  if (GetCurrentIndex() == -1)
  {
    SetCurrentIndex(0);
  }
  return *this;
}

ComboProperty& ComboProperty::operator<<(const std::vector<std::string>& str)
{
  m_values.insert(m_values.end(), str.begin(), str.end());
  if (GetCurrentIndex() == -1)
  {
    SetCurrentIndex(0);
  }
  return *this;
}

bool ComboProperty::operator==(const ComboProperty& other) const
{
  if (m_selected_indices != other.m_selected_indices)
  {
    return false;
  }
  if (m_values != other.m_values)
  {
    return false;
  }
  return true;
}

bool ComboProperty::operator!=(const ComboProperty& other) const
{
  return !(*this == other);
}

bool ComboProperty::operator<(const ComboProperty& other) const
{
  return std::tie(m_selected_indices, m_values)
         < std::tie(other.m_selected_indices, other.m_values);
}

bool ComboProperty::operator>=(const ComboProperty& other) const
{
  return (other < *this) || (*this == other);
}

std::string ComboProperty::ToString() const
{
  std::stringstream result;
  result << kValueKey << "[" << utils::VectorToString(m_values, kSeparator) << "]";
  result << " " << kSelectionKey << "["
         << utils::VectorToString(m_selected_indices, kSeparator) << "]";
  return result.str();
}

ComboProperty ComboProperty::CreateFromString(const std::string& str)
{
  auto optional_value_string = utils::GetKeyContent(str, kValueKey);
  auto optional_selection_string = utils::GetKeyContent(str, kSelectionKey);
  if (optional_value_string.has_value() && optional_selection_string.has_value())
  {
    auto values = utils::SplitString(optional_value_string.value(), kSeparator);
    ComboProperty result(values);
    if (!optional_selection_string.value().empty())
    {
      result.SetSelectedIndices(utils::StringToVector<std::int32_t>(
          optional_selection_string.value(), kSeparator));
    }
    return result;
  }

  throw RuntimeException("String doesn't represent ComboProperty");
}

}  // namespace mvvm
