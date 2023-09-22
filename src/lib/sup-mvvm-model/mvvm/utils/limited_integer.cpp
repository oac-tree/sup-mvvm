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

#include "limited_integer.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/container_utils.h>

#include <sstream>

namespace
{
const std::vector<mvvm::TypeCode> kSupportedTypes{
    mvvm::TypeCode::Int8,  mvvm::TypeCode::UInt8,  mvvm::TypeCode::Int16, mvvm::TypeCode::UInt16,
    mvvm::TypeCode::Int32, mvvm::TypeCode::UInt32, mvvm::TypeCode::Int64, mvvm::TypeCode::UInt64};
}

namespace mvvm
{
LimitedInteger::LimitedInteger(const variant_t &value, const variant_t &lower_bound,
                               const variant_t &upper_bound)
    : m_value(value), m_lower_bound(lower_bound), m_upper_bound(upper_bound)
{
  if (!utils::Contains(kSupportedTypes, GetTypeCode(m_value)))
  {
    throw RuntimeException("The value type [" + utils::TypeName(m_value) + "] is not supported");
  }

  if (utils::IsValid(m_lower_bound) && m_lower_bound.index() != value.index())
  {
    throw RuntimeException("The lower bound type=[" + utils::TypeName(m_lower_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(m_lower_bound)
                           + "]");
  }

  if (utils::IsValid(m_upper_bound) && m_upper_bound.index() != value.index())
  {
    throw RuntimeException("The upper bound type=[" + utils::TypeName(m_upper_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(m_upper_bound)
                           + "]");
  }
}

variant_t LimitedInteger::GetValue() const
{
  return m_value;
}

variant_t LimitedInteger::GetLowerBound() const
{
  return m_lower_bound;
}

variant_t LimitedInteger::GetUpperBound() const
{
  return m_upper_bound;
}

bool LimitedInteger::SetValue(const variant_t &value)
{
  return {};
}

bool LimitedInteger::Increment()
{
  return {};
}

bool LimitedInteger::Decrement()
{
  return {};
}

bool LimitedInteger::AddValue(const variant_t &value, bool as_much_as_possible)
{
  return {};
}

bool LimitedInteger::SubstractValue(const variant_t &value, bool as_much_as_possible)
{
  return {};
}

}  // namespace mvvm
