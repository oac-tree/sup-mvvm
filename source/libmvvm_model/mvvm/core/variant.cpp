/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/variant.h"

#include <map>

bool operator==(const datarole_t &lhs, const datarole_t &rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

namespace mvvm::utils
{

bool IsValid(const variant_t &value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

bool AreCompatible(const variant_t &var1, const variant_t &var2)
{
  // If one of them is invalid, they are considered to be compatible.
  if (!IsValid(var1) || !IsValid(var2))
  {
    return true;
  }

  // However, properly initialized variants of different types are considered to be incompatible.
  return var1.index() == var2.index();
}

std::string TypeName(const variant_t &variant)
{
  static std::map<int, std::string> type_name_map = {
      {0, constants::kUndefinedTypeName},    {1, constants::kBoolTypeName},
      {2, constants::kIntTypeName},          {3, constants::kDoubleTypeName},
      {4, constants::kStringTypeName},       {5, constants::kVectorDoubleTypeName},
      {6, constants::kComboPropertyTypeName}};
  return type_name_map[static_cast<int>(variant.index())];
}

}  // namespace mvvm::utils
