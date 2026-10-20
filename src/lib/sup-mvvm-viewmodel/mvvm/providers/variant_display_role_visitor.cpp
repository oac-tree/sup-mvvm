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
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/utils/string_utils.h>

namespace mvvm
{

namespace
{

//! Returns the text of float data with the precision and the notation of the item.
std::string FloatToString(double value, const SessionItem& item)
{
  const auto precision = utils::GetPrecision(item);
  return item.HasFlag(Appearance::kScientific) ? utils::ScientificDoubleToString(value, precision)
                                               : utils::FixedDoubleToString(value, precision);
}

}  // namespace

VariantDisplayRoleVisitor::VariantDisplayRoleVisitor(const SessionItem& item, role_t main_data_role)
    : m_item(item), m_main_data_role(main_data_role)
{
}

std::optional<std::string> VariantDisplayRoleVisitor::ToString() const
{
  auto value = m_item.Data(m_main_data_role);
  return std::visit(*this, value);
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(
    [[maybe_unused]] std::monostate value) const
{
  return {};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(boolean value) const
{
  return value ? std::string("True") : std::string("False");
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(char8 value) const
{
  return std::to_string(GetCharNumericValue(value));
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(int8 value) const
{
  return {std::to_string(value)};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(uint8 value) const
{
  return {std::to_string(static_cast<std::int32_t>(value))};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(int16 value) const
{
  return {std::to_string(static_cast<std::int32_t>(value))};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(uint16 value) const
{
  return {std::to_string(static_cast<uint32>(value))};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(int32 value) const
{
  return {std::to_string(value)};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(uint32 value) const
{
  return {std::to_string(value)};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(int64 value) const
{
  return {std::to_string(value)};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(uint64 value) const
{
  return {std::to_string(value)};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(float32 value) const
{
  return FloatToString(value, m_item);
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(float64 value) const
{
  return FloatToString(value, m_item);
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(std::string value) const
{
  return value;
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(
    const std::vector<double>& value) const
{
  return {utils::VectorToString(value, std::string(utils::kDefaultSeparator))};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(const ComboProperty& value) const
{
  return {value.GetLabel()};
}

std::optional<std::string> VariantDisplayRoleVisitor::operator()(
    const ExternalProperty& value) const
{
  return value.GetText();
}

}  // namespace mvvm
