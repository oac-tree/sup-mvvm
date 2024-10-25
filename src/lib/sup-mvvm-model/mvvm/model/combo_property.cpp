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

#include "combo_property.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_utils.h>

#include <numeric>

namespace
{

const std::string kValueSeparator = ";";
const std::string kSelectionSeparator = ",";
const std::string kMultipleSelectionLabel = "Multiple";
const std::string kNoneSelectionLabel = "None";

template <typename C>
std::string ToString(const C& container, const std::string& delim)
{
  auto delim_fold = [&delim](const std::string& a, const std::string& b) { return a + delim + b; };
  return container.empty() ? std::string()
                           : std::accumulate(std::next(container.begin()), container.end(),
                                             container[0], delim_fold);
}

}  // namespace

namespace mvvm
{

ComboProperty::ComboProperty() = default;

ComboProperty::ComboProperty(std::vector<std::string> values, const std::string& current_value)
    : m_values(std::move(values))
{
  if (!current_value.empty())
  {
    SetValue(current_value);
  }
  else
  {
    SetCurrentIndex(0);
  }
}

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

void ComboProperty::SetToolTips(const std::vector<std::string>& tooltips)
{
  m_tooltips = tooltips;
}

int ComboProperty::GetCurrentIndex() const
{
  return m_selected_indices.empty() ? -1 : m_selected_indices.at(0);
}

void ComboProperty::SetCurrentIndex(int index)
{
  if (index < 0 || index >= static_cast<int>(m_values.size()))
  {
    throw RuntimeException("Error in ComboProperty::setCurrentIndex(): invalid index");
  }
  m_selected_indices.clear();
  m_selected_indices.push_back(index);
}

std::vector<int> ComboProperty::GetSelectedIndices() const
{
  return m_selected_indices;
}

void ComboProperty::SetSelectedIndices(const std::vector<int>& selected_indices)
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

void ComboProperty::SetSelected(int index, bool value)
{
  if (index < 0 || index >= static_cast<int>(m_values.size()))
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
  else if (m_selected_indices.size() == 1)
  {
    return GetValue();
  }
  else
  {
    return kNoneSelectionLabel;
  }
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

std::string ComboProperty::GetStringOfValues() const
{
  return ToString(m_values, kValueSeparator);
}

void ComboProperty::SetValuesFromString(const std::string& values)
{
  auto current = GetValue();
  m_values = utils::SplitString(values, kValueSeparator);
  SetCurrentIndex(utils::Contains(m_values, current) ? utils::IndexOfItem(m_values, current) : 0);
}

std::string ComboProperty::GetStringOfSelections() const
{
  std::vector<std::string> text;
  text.reserve(m_selected_indices.size());
  for (auto index : m_selected_indices)
  {
    text.push_back(std::to_string(index));
  }

  return ToString(text, kSelectionSeparator);
}

void ComboProperty::SetSelectionFromString(const std::string& values)
{
  m_selected_indices.clear();
  if (values.empty())
  {
    return;
  }

  for (const auto& str : utils::SplitString(values, kSelectionSeparator))
  {
    const int num = std::stoi(str);
    SetSelected(num, true);
  }
}

}  // namespace mvvm
