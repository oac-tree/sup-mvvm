/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/limits_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/limits.h>
#include <mvvm/utils/string_utils.h>

#include <sstream>

namespace
{
const std::string kTextLimitless = "limitless";
const std::string kTextPositive = "positive";
const std::string kTextNonnegative = "nonnegative";
const std::string kTextLowerlimited = "lowerlimited";
const std::string kTextUpperlimited = "upperlimited";
const std::string kTextLimited = "limited";

const std::string separator = ";";

std::string NumberToString(double number)
{
  return mvvm::utils::DoubleToString(number);
}

std::string NumberToString(int number)
{
  return std::to_string(number);
}

template <typename T>
std::string ToStringT(const mvvm::Limits<T> &limits)
{
  if (limits.IsLimitless())
  {
    return kTextLimitless;
  }

  if (limits.IsPositive())
  {
    return kTextPositive;
  }

  if (limits.IsNonnegative())
  {
    return kTextNonnegative;
  }

  if (limits.IsLowerLimited())
  {
    return kTextLowerlimited + separator + NumberToString(limits.GetLowerLimit());
  }

  if (limits.IsUpperLimited())
  {
    return kTextUpperlimited + separator + NumberToString(limits.GetUpperLimit());
  }

  if (limits.IsLimited())
  {
    return kTextLimited + separator + NumberToString(limits.GetLowerLimit()) + separator
           + NumberToString(limits.GetUpperLimit());
  }

  if (limits.IsLimitless())
  {
    return kTextLimitless;
  }

  throw mvvm::LogicErrorException("Unknown limits");
}

}  // namespace

namespace mvvm
{

std::string ToString(const Limits<int> &limits)
{
  return ToStringT(limits);
}

std::string ToString(const Limits<double> &limits)
{
  return ToStringT(limits);
}

}  // namespace mvvm
