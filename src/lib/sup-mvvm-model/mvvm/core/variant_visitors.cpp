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
namespace mvvm
{

std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound,
                                          const variant_t& upper_bound)
{
  VariantLimits32Visitor<int> visitor;
  visitor.m_lower_bound = lower_bound;
  visitor.m_upper_bound = upper_bound;

  return std::visit(visitor, value);
}

}  // namespace mvvm
