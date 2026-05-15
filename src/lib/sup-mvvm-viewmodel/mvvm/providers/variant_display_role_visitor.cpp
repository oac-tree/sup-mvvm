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

#include "variant_display_role_visitor.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/string_utils.h>

namespace mvvm
{

VariantDisplayRoleVisitor::VariantDisplayRoleVisitor(const SessionItem& item, role_t main_data_role)
    : m_item(item), m_main_data_role(main_data_role)
{
}

std::string VariantDisplayRoleVisitor::ToString() const
{
  auto value = m_item.Data(m_main_data_role);
  return std::visit(*this, value);
}

std::string VariantDisplayRoleVisitor::operator()([[maybe_unused]] std::monostate value) const
{
  return {};
}

std::string VariantDisplayRoleVisitor::operator()(boolean value) const
{
  auto str = utils::FromBool(value);
  if (!str.empty())
  {
    str[0] = std::toupper(str[0]);
  }
  return str;
}

std::string VariantDisplayRoleVisitor::operator()(char8 value) const
{
  return std::to_string(static_cast<std::int32_t>(value));
}

std::string VariantDisplayRoleVisitor::operator()(int8 value) const
{
  return {std::to_string(value)};
}

std::string VariantDisplayRoleVisitor::operator()(uint8 value) const
{
  return {std::to_string(static_cast<std::int32_t>(value))};
}

std::string VariantDisplayRoleVisitor::operator()(int16 value) const
{
  return {std::to_string(static_cast<std::int32_t>(value))};
}

std::string VariantDisplayRoleVisitor::operator()(uint16 value) const
{
  return {std::to_string(static_cast<uint32>(value))};
}

std::string VariantDisplayRoleVisitor::operator()(int32 value) const
{
  return {std::to_string(value)};
}

std::string VariantDisplayRoleVisitor::operator()(uint32 value) const
{
  return {std::to_string(value)};
}

std::string VariantDisplayRoleVisitor::operator()(int64 value) const
{
  return {std::to_string(value)};
}

std::string VariantDisplayRoleVisitor::operator()(uint64 value) const
{
  return {std::to_string(value)};
}

std::string VariantDisplayRoleVisitor::operator()(float32 value) const
{
  const std::int32_t precision = constants::kDefaultDoubleDecimals;
  return utils::DoubleToString(value, precision);
}

std::string VariantDisplayRoleVisitor::operator()(float64 value) const
{
  const std::int32_t precision = constants::kDefaultDoubleDecimals;
  return utils::DoubleToString(value, precision);
}

std::string VariantDisplayRoleVisitor::operator()(std::string value) const
{
  return value;
}

std::string VariantDisplayRoleVisitor::operator()(const std::vector<double>& value) const
{
  return {utils::VectorToString(value, std::string(utils::kDefaultSeparator))};
}

std::string VariantDisplayRoleVisitor::operator()(const ComboProperty& value) const
{
  return {value.GetLabel()};
}

std::string VariantDisplayRoleVisitor::operator()(const ExternalProperty& value) const
{
  return value.GetText();
}

}  // namespace mvvm
