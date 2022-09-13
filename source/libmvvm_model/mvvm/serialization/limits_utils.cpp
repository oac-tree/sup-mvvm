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
T NumberFromString(const std::string &str)
{
  T var;
  std::istringstream iss(str);
  iss.imbue(std::locale::classic());
  iss >> var;
  return var;
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

  throw mvvm::LogicErrorException("Unknown limits");
}

template <typename T>
mvvm::Limits<T> LimitsFromStringT(const std::string &str)
{
  auto parts = mvvm::utils::SplitString(str, separator);

  if (parts.empty())
  {
    throw mvvm::LogicErrorException("Can't parse Limits representation");
  }

  auto key = parts[0];

  if (key == kTextLimitless)
  {
    return mvvm::Limits<T>::CreateLimitless();
  }

  if (key == kTextPositive)
  {
    return mvvm::Limits<T>::CreatePositive();
  }

  if (key == kTextNonnegative)
  {
    return mvvm::Limits<T>::CreateNonnegative();
  }

  if (parts.size() < 1)
  {
    throw mvvm::LogicErrorException("Can't parse Limits representation");
  }

  if (key == kTextLowerlimited)
  {
    return mvvm::Limits<T>::CreateLowerLimited(NumberFromString<T>(parts[1]));
  }

  if (key == kTextUpperlimited)
  {
    return mvvm::Limits<T>::CreateUpperLimited(NumberFromString<T>(parts[1]));
  }

  if (parts.size() < 2)
  {
    throw mvvm::LogicErrorException("Can't parse Limits representation");
  }

  if (key == kTextLimited)
  {
    return mvvm::Limits<T>::CreateLimited(NumberFromString<T>(parts[1]),
                                          NumberFromString<T>(parts[2]));
  }

  throw mvvm::LogicErrorException("Unknown limits");
}

}  // namespace

namespace mvvm::utils
{

std::string ToString(const Limits<int> &limits)
{
  return ToStringT<int>(limits);
}

Limits<int> IntLimitsFromString(const std::string &str)
{
  return LimitsFromStringT<int>(str);
}

std::string ToString(const Limits<double> &limits)
{
  return ToStringT<double>(limits);
}

Limits<double> RealLimitsFromString(const std::string &str)
{
  return LimitsFromStringT<double>(str);
}

}  // namespace mvvm::utils
