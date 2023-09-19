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

#include "item_utils.h"
#include "sessionitem.h"

#include <mvvm/core/exceptions.h>

#include <sstream>

namespace
{

/**
 * @brief Validate if type of the data matches item's data and thow an exception if it is not the
 * case.
 *
 * @param item An item with some data stored.
 * @param value The data from outside to compare with.
 * @param The role of item's data.
 */
void ValidateDataType(const mvvm::SessionItem &item, const mvvm::variant_t &value, int role)
{
  if (item.HasData(role) && item.Data(role).index() != value.index())
  {
    std::ostringstream ostr;
    ostr << "The item data type [" << item.Data(role).index() << "] for role [" << role
         << "] doesn't coincide with value type [" << value.index() << "]\n";
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
  ValidateDataType(item, value, DataRole::kData);
  ValidateDataType(item, value, DataRole::kLowerLimit);
  ValidateDataType(item, value, DataRole::kUpperLimit);

//  if (HasLowerLimit(item) && value < GetLowerLimit(item))
//  {
//    return false;
//  }
//  if (HasUpperLimit(item) && value >= m_upper_limit.value())
//  {
//    return false;
//  }
//  return true;

}

}  // namespace mvvm
