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

#include "variant_value_visitor.h"

#include <mvvm/utils/string_utils.h>

namespace mvvm
{

std::string VariantValueVisitor::operator()(std::monostate value)
{
  (void)value;
  return {};
}

std::string VariantValueVisitor::operator()(boolean value)
{
  return mvvm::utils::FromBool(value);
}

std::string VariantValueVisitor::operator()(char8 value)
{
  return std::to_string(value);
}

std::string VariantValueVisitor::operator()(int8 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(uint8 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(int16 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(uint16 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(int32 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(uint32 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(int64 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(uint64 value)
{
  return {std::to_string(value)};
}

std::string VariantValueVisitor::operator()(float32 value)
{
  return mvvm::utils::DoubleToString(value);
}

std::string VariantValueVisitor::operator()(float64 value)
{
  return mvvm::utils::DoubleToString(value);
}

std::string VariantValueVisitor::operator()(std::string value)
{
  return value;
}

std::string VariantValueVisitor::operator()(const std::vector<double> &value)
{
  return {mvvm::utils::ToCommaSeparatedString(value)};
}

std::string VariantValueVisitor::operator()(const ComboProperty &value)
{
  return {value.GetStringOfValues()};
}

std::string VariantValueVisitor::operator()(const ExternalProperty &value)
{
  return value.ToString();
}

}  // namespace mvvm
