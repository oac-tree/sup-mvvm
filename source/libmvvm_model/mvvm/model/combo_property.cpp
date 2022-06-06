/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/combo_property.h"

#include "mvvm/utils/container_utils.h"

#include <sstream>
#include <stdexcept>

namespace
{
const std::string value_separator = ";";
const std::string selection_separator = ",";
const std::string multiple_label = "Multiple";
const std::string none_label = "None";

template <typename C, typename T>
std::string ToString(const C& container, const T& delim)
{
  std::stringstream result;
  for (auto it = container.begin(); it != container.end(); ++it)
  {
    result << *it;
    if (std::distance(it, container.end()) != 1)
    {
      result << delim;
    }
  }
  return result.str();
}

std::vector<std::string> Tokenize(const std::string& str, const std::string& delimeter)
{
  std::vector<std::string> result;
  size_t start = str.find_first_not_of(delimeter);
  size_t end = start;

  while (start != std::string::npos)
  {
    // Find next occurence of delimiter
    end = str.find(delimeter, start);
    // Push back the token found into vector
    result.push_back(str.substr(start, end - start));
    // Skip all occurences of the delimiter to find new start
    start = str.find_first_not_of(delimeter, end);
  }
  return result;
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
    throw std::runtime_error(
        "ComboProperty::setValue() -> Error. Combo doesn't contain "
        "value "
        + name);
  }
  SetCurrentIndex(utils::IndexOfItem(m_values, name));
}

std::vector<std::string> ComboProperty::GetValues() const
{
  return m_values;
}

//! Sets new list of values. Current value will be preserved, if exists in a new list.

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

//! returns list of tool tips for all values
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
    throw std::runtime_error(
        "ComboProperty::setCurrentIndex(int index) -> Error. "
        "Invalid index");
  }
  m_selected_indices.clear();
  m_selected_indices.push_back(index);
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
  return m_selected_indices < other.m_selected_indices && m_values < other.m_values;
}

//! Returns a single string containing values delimited with ';'.

std::string ComboProperty::GetStringOfValues() const
{
  return ToString(m_values, value_separator);
}

//! Sets values from the string containing delimeter ';'.

void ComboProperty::SetStringOfValues(const std::string& values)
{
  auto current = GetValue();
  m_values = Tokenize(values, value_separator);
  SetCurrentIndex(utils::Contains(m_values, current) ? utils::IndexOfItem(m_values, current) : 0);
}

//! Returns vector of selected indices.

std::vector<int> ComboProperty::GetSelectedIndices() const
{
  return m_selected_indices;
}

//! Returns list of string with selected values;

std::vector<std::string> ComboProperty::SetSelectedValues() const
{
  std::vector<std::string> result;
  for (auto index : m_selected_indices)
  {
    result.push_back(m_values.at(static_cast<size_t>(index)));
  }
  return result;
}

//! Sets given index selection flag.
//! If false, index will be excluded from selection.

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

//! Return string with coma separated list of selected indices.

std::string ComboProperty::GetStringOfSelections() const
{
  std::vector<std::string> text;
  for (auto index : m_selected_indices)
  {
    text.push_back(std::to_string(index));
  }
  return ToString(text, selection_separator);
}

//! Sets selected indices from string.

void ComboProperty::SetStringOfSelections(const std::string& values)
{
  m_selected_indices.clear();
  if (values.empty())
  {
    return;
  }

  for (const auto& str : Tokenize(values, selection_separator))
  {
    int num = std::stoi(str);
    SetSelected(num, true);
  }
}

//! Returns the label to show.

std::string ComboProperty::GetLabel() const
{
  if (m_selected_indices.size() > 1)
  {
    return multiple_label;
  }
  else if (m_selected_indices.size() == 1)
  {
    return GetValue();
  }
  else
  {
    return none_label;
  }
}

}  // namespace mvvm
