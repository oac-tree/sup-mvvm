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

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/string_utils.h>

#include <functional>
#include <map>

// ----------------------------------------------------------------------------
// Declarations of constants and helper methods in anonymous namespace.
// ----------------------------------------------------------------------------

namespace mvvm
{

namespace
{

const std::string kSelectionsAttributeKey = "selections";

//! Aggregates call backs for conversion between role_data_t and TreeData.
struct Converters
{
  std::function<tree_data_t(const role_data_t& role_data)> roledata_to_treedata;
  std::function<role_data_t(const tree_data_t& treedata)> treedata_to_roledata;
};

//! Returns role attribute.
role_t GetRole(const tree_data_t& tree_data);

//! Returns variant type name.
std::string GetTypeName(const tree_data_t& tree_data);

//! Converts role_data_t to the TreeData object, basic implementation that fits most needs.
tree_data_t from_roledata_default_impl(const mvvm::role_data_t& role_data);

//! Converts TreeData to role_data_t, basic implementation that fits most needs.
mvvm::role_data_t to_role_data_default_impl(const tree_data_t& tree_data);

//! Converts role_data_t holding ComboProperty to the TreeData object.
tree_data_t from_combo_property(const role_data_t& role_data);

//! Converts TreeData to role_data_t holding ComboProperty data.
mvvm::role_data_t to_combo_property(const tree_data_t& tree_data);

//! Returns map of all defined converters.
std::map<std::string, Converters> GetConverters();

}  // namespace

namespace TreeDataVariantConverter
{

role_data_t ToRoleData(const tree_data_t& tree_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  if (!IsDataRoleConvertible(tree_data))
  {
    throw RuntimeException("Error in variant converter: invalid TreeData object");
  }

  const auto type_name = GetTypeName(tree_data);
  auto iter = converters.find(type_name);
  if (iter == converters.end())
  {
    throw RuntimeException("Error in variant converter: can't find type name [" + type_name + "]");
  }

  return iter->second.treedata_to_roledata(tree_data);
}

tree_data_t ToTreeData(const role_data_t& role_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  const std::string type_name = utils::TypeName(role_data.second);

  auto iter = converters.find(type_name);
  if (iter == converters.end())
  {
    throw RuntimeException("Error in variant converter: can't find type name [" + type_name + "]");
  }
  return iter->second.roledata_to_treedata(role_data);
}

}  // namespace TreeDataVariantConverter

namespace
{

role_t GetRole(const tree_data_t& tree_data)
{
  return std::stoi(tree_data.GetAttribute(constants::kRoleAttributeKey));
}

std::string GetTypeName(const mvvm::tree_data_t& tree_data)
{
  return tree_data.GetAttribute(constants::kTypeAttributeKey);
}

tree_data_t from_roledata_default_impl(const mvvm::role_data_t& role_data)
{
  tree_data_t result(constants::kVariantElementType);
  result.AddAttribute(constants::kTypeAttributeKey, mvvm::utils::TypeName(role_data.second));
  result.AddAttribute(constants::kRoleAttributeKey, std::to_string(role_data.first));
  result.SetContent(mvvm::utils::ValueToString(role_data.second));
  return result;
}

mvvm::role_data_t to_role_data_default_impl(const tree_data_t& tree_data)
{
  (void)tree_data;
  const auto type_code = GetTypeCodeFromTypeName(GetTypeName(tree_data));
  const variant_t value = utils::ValueFromString(type_code, tree_data.GetContent());
  return {GetRole(tree_data), value};
}

tree_data_t from_combo_property(const role_data_t& role_data)
{
  tree_data_t result(constants::kVariantElementType);
  result.AddAttribute(constants::kRoleAttributeKey, std::to_string(role_data.first));
  result.AddAttribute(constants::kTypeAttributeKey, mvvm::constants::kComboPropertyTypeName);
  auto combo = std::get<mvvm::ComboProperty>(role_data.second);
  result.SetContent(combo.ToString());
  return result;
}

role_data_t to_combo_property(const tree_data_t& tree_data)
{
  const auto combo = ComboProperty::CreateFromString(tree_data.GetContent());
  return {GetRole(tree_data), combo};
}

std::map<std::string, Converters> GetConverters()
{
  const static std::map<std::string, Converters> result = {
      {constants::kEmptyTypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kBooleanTypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kChar8TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kInt8TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kUInt8TypeName, {from_roledata_default_impl, to_role_data_default_impl}},

      {constants::kInt16TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kUInt16TypeName, {from_roledata_default_impl, to_role_data_default_impl}},

      {constants::kInt32TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kUInt32TypeName, {from_roledata_default_impl, to_role_data_default_impl}},

      {constants::kInt64TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kUInt64TypeName, {from_roledata_default_impl, to_role_data_default_impl}},

      {constants::kStringTypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kFloat32TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kFloat64TypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kVectorDoubleTypeName, {from_roledata_default_impl, to_role_data_default_impl}},
      {constants::kComboPropertyTypeName, {from_combo_property, to_combo_property}},
      {constants::kExternalPropertyTypeName, {from_roledata_default_impl, to_role_data_default_impl}}};

  return result;
}

}  // namespace

}  // namespace mvvm
