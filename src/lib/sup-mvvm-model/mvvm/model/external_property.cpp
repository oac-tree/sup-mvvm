/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "external_property.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/string_utils.h>

#include <sstream>
#include <tuple>

namespace
{
constexpr std::string_view delim(";");
}

namespace mvvm
{

ExternalProperty::ExternalProperty() = default;

ExternalProperty::ExternalProperty(std::string text, std::string color, std::string identifier)
    : m_text(std::move(text)), m_color(std::move(color)), m_identifier(std::move(identifier))
{
}

ExternalProperty ExternalProperty::CreateUndefined()
{
  return {"Undefined", "red"};
}

std::string ExternalProperty::GetText() const
{
  return m_text;
}

std::string ExternalProperty::GetColorName() const
{
  return m_color;
}

std::string ExternalProperty::GetIdentifier() const
{
  return m_identifier;
}

bool ExternalProperty::IsEmpty() const
{
  return m_identifier.empty() && m_text.empty() && m_color.empty();
}

bool operator==(const ExternalProperty& lhs, const ExternalProperty& rhs)
{
  return (lhs.GetIdentifier() == rhs.GetIdentifier()) && (lhs.GetText() == rhs.GetText())
         && (lhs.GetColorName() == rhs.GetColorName());
}

bool operator!=(const ExternalProperty& lhs, const ExternalProperty& rhs)
{
  return !(lhs == rhs);
}

bool operator<(const ExternalProperty& lhs, const ExternalProperty& rhs)
{
  return std::tie(lhs.m_identifier, lhs.m_text, lhs.m_color)
         < std::tie(rhs.m_identifier, rhs.m_text, rhs.m_color);
}

bool operator>=(const ExternalProperty& lhs, const ExternalProperty& rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

//! Converts class into its string representation.
//! - ExternalProperty("","","") becomes ";;"
//! - ExternalProperty("text","","") becomes "text;;"
//! - ExternalProperty("text","color","identifier") becomes "text;color;identifier"

std::string ExternalProperty::ToString() const
{
  std::stringstream result;
  result << m_text << delim << m_color << delim << m_identifier;
  return result.str();
}

ExternalProperty ExternalProperty::CreateFromString(const std::string& text)
{
  auto parts = utils::SplitString(text, std::string(delim));
  if (parts.size() != 3)
  {
    throw RuntimeException("Error in ExternalProperty: malformed string");
  }
  return {parts.at(0), parts.at(1), parts.at(2)};
}

}  // namespace mvvm
