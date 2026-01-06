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

#include "tree_data_variant_converter.h"

#include "tree_data.h"  // IWYU pragma: export
#include "tree_data_constants.h"
#include "tree_data_helper.h"

#include <mvvm/utils/string_utils.h>

namespace mvvm
{

namespace
{

/**
 * @brief Returns role attribute.
 */
role_t GetRole(const tree_data_t& tree_data)
{
  return std::stoi(tree_data.GetAttribute(constants::kRoleAttributeKey));
}

/**
 * @brief Returns type attribute.
 */
std::string GetTypeName(const mvvm::tree_data_t& tree_data)
{
  return tree_data.GetAttribute(constants::kTypeAttributeKey);
}

}  // namespace

namespace TreeDataVariantConverter
{

role_data_t ToRoleData(const tree_data_t& tree_data)
{
  if (!IsDataRoleConvertible(tree_data))
  {
    throw RuntimeException("Error in variant converter: invalid TreeData object");
  }

  const auto type_code = GetTypeCodeFromTypeName(GetTypeName(tree_data));
  const variant_t value = utils::ValueFromString(type_code, tree_data.GetContent());
  return {GetRole(tree_data), value};
}

tree_data_t ToTreeData(const role_data_t& role_data)
{
  tree_data_t result(constants::kVariantElementType);
  result.AddAttribute(constants::kTypeAttributeKey, mvvm::utils::TypeName(role_data.second));
  result.AddAttribute(constants::kRoleAttributeKey, std::to_string(role_data.first));
  result.SetContent(mvvm::utils::ValueToString(role_data.second));
  return result;
}

}  // namespace TreeDataVariantConverter

}  // namespace mvvm
