/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "variant_visitors.h"

#include <limits>

namespace
{

/**
 * @brief Returns pair of integer representing
 */

template <typename T>
std::pair<int, int> GetMinMaxInt32Pair()
{
  return {std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};
}
}  // namespace

namespace mvvm
{

std::pair<int, int> VariantLimits32Visitor::operator()(std::monostate)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(boolean value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(char8 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(int8 value)
{
  return GetMinMaxInt32Pair<int8>();
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint8 value)
{
  return GetMinMaxInt32Pair<uint8>();
}

std::pair<int, int> VariantLimits32Visitor::operator()(int16 value)
{
  return GetMinMaxInt32Pair<int16>();
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint16 value)
{
  return GetMinMaxInt32Pair<uint16>();
}

std::pair<int, int> VariantLimits32Visitor::operator()(int32 value)
{
  return GetMinMaxInt32Pair<int32>();
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint32 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(int64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(float32 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(float64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(std::string value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(std::vector<double> value)
{
  throw mvvm::RuntimeException("Visitor for vector<double> is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(ComboProperty value)
{
  throw mvvm::RuntimeException("Visitor for ComboProperty is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(ExternalProperty value)
{
  throw mvvm::RuntimeException("Visitor for ExternalProperty is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<int> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<int64> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<double> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> GetInt32MinMaxNumeric(const variant_t &value)
{
  return std::visit(VariantLimits32Visitor{}, value);
}

}  // namespace mvvm
