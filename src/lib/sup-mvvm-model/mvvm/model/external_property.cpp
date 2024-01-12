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

#include "external_property.h"

#include <mvvm/utils/string_utils.h>
#include <mvvm/core/exceptions.h>

#include <sstream>
#include <tuple>

namespace
{
const std::string delim(";");
}

namespace mvvm
{

ExternalProperty::ExternalProperty() = default;

ExternalProperty::ExternalProperty(std::string text, std::string color, std::string identifier)
    : m_text(std::move(text)), m_color(std::move(color)), m_identifier(std::move(identifier))
{
}

//! Creates undefined property, which will be shown in views as red box with text "Undefined".

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

bool ExternalProperty::operator==(const ExternalProperty& other) const
{
  return m_identifier == other.m_identifier && m_text == other.m_text && m_color == other.m_color;
}

bool ExternalProperty::operator!=(const ExternalProperty& other) const
{
  return !(*this == other);
}

bool ExternalProperty::operator<(const ExternalProperty& other) const
{
  return std::tie(m_identifier, m_text, m_color)
         < std::tie(other.m_identifier, other.m_text, other.m_color);
}

bool ExternalProperty::operator>=(const ExternalProperty& other) const
{
  return (other < *this) || (*this == other);
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
  auto parts = utils::SplitString(text, delim);
  if (parts.size() != 3)
  {
    throw RuntimeException("Error in ExternalProperty: malformed string");
  }
  return {parts.at(0), parts.at(1), parts.at(2)};
}

}  // namespace mvvm
