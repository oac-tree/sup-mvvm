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

namespace mvvm
{
LimitedInteger::LimitedInteger(const variant_t &value, const variant_t &lower_bound,
                               const variant_t &upper_bound)
    : m_value(value), m_lower_bound(lower_bound), m_upper_bound(upper_bound)
{
}

variant_t LimitedInteger::GetValue() const
{
  return {};
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
