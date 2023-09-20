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

#include "item_limits_helper.h"

#include "sessionitem.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/core/variant_visitors.h>

#include <sstream>

namespace
{

/**
 * @brief Validate if type of the value matches item's variant and throw an exception if it is not
 * the case.
 *
 * @param item An item with some data stored.
 * @param value The data from outside to compare with.
 * @param The role of item's data.
 *
 * @details If item doesn't contain given role, do nothing.
 */
void ValidateDataType(const mvvm::SessionItem &item, const mvvm::variant_t &value, int role)
{
  if (item.HasData(role) && item.Data(role).index() != value.index())
  {
    std::ostringstream ostr;
    ostr << "The item data type=[" << item.Data(role).index() << "] for role=[" << role
         << "] doesn't coincide with value type=[" << value.index() << "]\n";
    throw mvvm::LogicErrorException(ostr.str());
  }
}

}  // namespace

namespace mvvm
{

bool HasLowerLimit(const SessionItem &item)
{
  return item.HasData(DataRole::kLowerLimit);
}

variant_t GetLowerLimit(const SessionItem &item)
{
  return item.Data(DataRole::kLowerLimit);
}

bool HasUpperLimit(const SessionItem &item)
{
  return item.HasData(DataRole::kUpperLimit);
}

variant_t GetUpperLimit(const SessionItem &item)
{
  return item.Data(DataRole::kUpperLimit);
}

bool IsInRange(const SessionItem &item, const variant_t &value)
{
  // type of value being tested should match the type of variant stored in all related roles
  ValidateDataType(item, value, DataRole::kData);
  ValidateDataType(item, value, DataRole::kLowerLimit);
  ValidateDataType(item, value, DataRole::kUpperLimit);

  if (HasLowerLimit(item) && value < GetLowerLimit(item))
  {
    return false;
  }

  if (HasUpperLimit(item) && value >= GetUpperLimit(item))
  {
    return false;
  }

  return true;
}

bool IsLimitless(const SessionItem &item)
{
  return !HasLowerLimit(item) && !HasUpperLimit(item);
}
bool IsLimited(const SessionItem &item)
{
  return HasLowerLimit(item) && HasUpperLimit(item);
}

void SetLowerLimited(const variant_t &bound_value, SessionItem &item)
{
  // type of bound value should match the type of variant stored in kData role
  ValidateDataType(item, bound_value, DataRole::kData);

  RemoveLimits(item);

  item.SetData(bound_value, DataRole::kLowerLimit);
}

void SetUpperLimited(const variant_t &bound_value, SessionItem &item)
{
  // type of bound value should match the type of variant stored in kData role
  ValidateDataType(item, bound_value, DataRole::kData);

  RemoveLimits(item);

  item.SetData(bound_value, DataRole::kUpperLimit);
}

void RemoveLimits(SessionItem &item)
{
  if (item.HasData(DataRole::kLowerLimit))
  {
    // setting empty variant will remove role, if it is not present
    item.SetData(mvvm::variant_t{}, DataRole::kLowerLimit);
  }

  if (item.HasData(DataRole::kUpperLimit))
  {
    // setting empty variant will remove role, if it is not present
    item.SetData(mvvm::variant_t{}, DataRole::kUpperLimit);
  }
}

void SetLimited(const variant_t &left_bound_value, const variant_t &right_bound_value,
                SessionItem &item)
{
  if (left_bound_value.index() != right_bound_value.index())
  {
    throw LogicErrorException("Attempt to set limits with different underlying data type");
  }

  // type of bound value should match the type of variant stored in kData role
  ValidateDataType(item, left_bound_value, DataRole::kData);
  ValidateDataType(item, right_bound_value, DataRole::kData);

  item.SetData(left_bound_value, DataRole::kLowerLimit);
  item.SetData(right_bound_value, DataRole::kUpperLimit);
}

std::pair<int, int> GetInt32Limits(const SessionItem &item)
{
  if (!item.HasData(DataRole::kData))
  {
    throw LogicErrorException("Given item doesn't contain any data");
  }

  auto lower_bound = GetLowerLimit(item);
  auto upper_bound = GetUpperLimit(item);

  return GetInt32MinMaxNumeric(item.Data(), lower_bound, upper_bound);
}

}  // namespace mvvm
