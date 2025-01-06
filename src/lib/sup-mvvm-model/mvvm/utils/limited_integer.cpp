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

#include "limited_integer.h"

#include <algorithm>

namespace mvvm
{

bool IsSupportedVariant(const variant_t &variant)
{
  static const std::vector<mvvm::TypeCode> kSupportedTypes{
      mvvm::TypeCode::Char8,  mvvm::TypeCode::Int8,   mvvm::TypeCode::UInt8,
      mvvm::TypeCode::Int16,  mvvm::TypeCode::UInt16, mvvm::TypeCode::Int32,
      mvvm::TypeCode::UInt32, mvvm::TypeCode::Int64,  mvvm::TypeCode::UInt64};

  return std::find(kSupportedTypes.begin(), kSupportedTypes.end(), GetTypeCode(variant))
         != kSupportedTypes.end();
}

void ValidateBounds(const variant_t &value, const variant_t &lower_bound,
                    const variant_t &upper_bound)
{
  if (!IsSupportedVariant(value))
  {
    throw RuntimeException("The value type [" + utils::TypeName(value) + "] is not supported");
  }

  if (utils::IsValid(lower_bound) && lower_bound.index() != value.index())
  {
    throw RuntimeException("The lower bound type=[" + utils::TypeName(lower_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(lower_bound)
                           + "]");
  }

  if (utils::IsValid(upper_bound) && upper_bound.index() != value.index())
  {
    throw RuntimeException("The upper bound type=[" + utils::TypeName(upper_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(upper_bound)
                           + "]");
  }
}

}  // namespace mvvm
