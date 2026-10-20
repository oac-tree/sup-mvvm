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

#include "limited_float_helper.h"

#include "limited_float.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <optional>
#include <string>

namespace mvvm
{

namespace
{

/**
 * @brief Returns the float stored in the variant, empty if the variant is invalid.
 *
 * @details Throws if the variant holds another type.
 */
template <typename FloatT>
std::optional<FloatT> GetOptionalFloat(const variant_t& variant, const std::string& name)
{
  if (!utils::IsValid(variant))
  {
    return std::nullopt;
  }

  if (!std::holds_alternative<FloatT>(variant))
  {
    throw RuntimeException("The " + name + " type=[" + utils::TypeName(variant)
                           + "] doesn't coincide with float type=["
                           + utils::TypeName(variant_t{FloatT{}}) + "]");
  }

  return std::get<FloatT>(variant);
}

/**
 * @brief Creates specialised limited float, for given input value and bounds.
 *
 * @details The default_value of a nullable float, or the value of a non-nullable one, holds
 * FloatT.
 */
template <typename FloatT>
std::unique_ptr<ILimitedFloat> FactoryFunc(const variant_t& value, const variant_t& default_value,
                                           const variant_t& lower_bound,
                                           const variant_t& upper_bound, bool is_nullable)
{
  const auto lower = GetOptionalFloat<FloatT>(lower_bound, "lower bound");
  const auto upper = GetOptionalFloat<FloatT>(upper_bound, "upper bound");

  if (is_nullable)
  {
    return std::make_unique<LimitedFloat<FloatT>>(GetOptionalFloat<FloatT>(value, "value"),
                                                  std::get<FloatT>(default_value), lower, upper);
  }
  return std::make_unique<LimitedFloat<FloatT>>(std::get<FloatT>(value), lower, upper);
}

/**
 * @brief Creates limited float of the type of default_value.
 */
std::unique_ptr<ILimitedFloat> CreateLimitedFloatImpl(const variant_t& value,
                                                      const variant_t& default_value,
                                                      const variant_t& lower_bound,
                                                      const variant_t& upper_bound,
                                                      bool is_nullable)
{
  if (std::holds_alternative<float32>(default_value))
  {
    return FactoryFunc<float32>(value, default_value, lower_bound, upper_bound, is_nullable);
  }

  if (std::holds_alternative<float64>(default_value))
  {
    return FactoryFunc<float64>(value, default_value, lower_bound, upper_bound, is_nullable);
  }

  throw RuntimeException("Can't create limited float for type=[" + utils::TypeName(default_value)
                         + "]");
}

}  // namespace

std::unique_ptr<ILimitedFloat> CreateLimitedFloat(const variant_t& value,
                                                  const variant_t& lower_bound,
                                                  const variant_t& upper_bound)
{
  return CreateLimitedFloatImpl(value, value, lower_bound, upper_bound, false);
}

std::unique_ptr<ILimitedFloat> CreateNullableLimitedFloat(const variant_t& value,
                                                          const variant_t& default_value,
                                                          const variant_t& lower_bound,
                                                          const variant_t& upper_bound)
{
  return CreateLimitedFloatImpl(value, default_value, lower_bound, upper_bound, true);
}

}  // namespace mvvm
