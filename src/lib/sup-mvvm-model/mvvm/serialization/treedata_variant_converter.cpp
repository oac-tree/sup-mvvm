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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "treedata_variant_converter.h"

#include "tree_data_helper.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/container_utils.h>
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
const std::string kVariantElementType = "Variant";
const std::string kRoleAttributeKey = "role";
const std::string kTypeAttributeKey = "type";
const std::string kSelectionsAttributeKey = "selections";

//! Aggregates call backs for convertion between role_data_t and TreeData.
struct Converters
{
  std::function<tree_data_t(const role_data_t& role_data)> roledata_to_treedata;
  std::function<role_data_t(const tree_data_t& treedata)> treedata_to_roledata;
};

////! Returns vector of attributes which TreeData object should have.
// std::vector<std::string> GetExpectedAttributeKeys();

//! Returns role attribute.
int GetRole(const tree_data_t &tree_data);

//! Returns variant type name.
std::string GetTypeName(const tree_data_t& tree_data);

//! Converts role_data_t holdingto the TreeData object, basic implementation that fits most needs.
tree_data_t from_roledata_default_impl(const mvvm::role_data_t& role_data);

//! Converts TreeData to role_data_t, basic implementation that feets most needs
mvvm::role_data_t to_undefined(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding boolean.
mvvm::role_data_t to_bool(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding integer data.
template<typename T>
mvvm::role_data_t to_int(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding string data.
mvvm::role_data_t to_string(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding double data.
template<typename T>
mvvm::role_data_t to_double(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding vector<double>.
mvvm::role_data_t to_vector_double(const tree_data_t& tree_data);

//! Converts role_data_t holding ComboProperty to the TreeData object.
tree_data_t from_combo_property(const role_data_t& role_data);

//! Converts TreeData to role_data_t holding ComboProperty data.
mvvm::role_data_t to_combo_property(const tree_data_t& tree_data);

//! Converts TreeData to role_data_t holding ExternalProperty data.
mvvm::role_data_t to_external_property(const tree_data_t& tree_data);

//! Returns map of all defined converters.
std::map<std::string, Converters> GetConverters();

}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

bool IsDataRoleConvertible(const tree_data_t& tree_data)
{
  auto attribute_names = GetAttributeNames(tree_data);
  const bool correct_attributes = utils::Contains(attribute_names, kRoleAttributeKey)
                                  && utils::Contains(attribute_names, kTypeAttributeKey);
  return tree_data.GetNodeName() == kVariantElementType && correct_attributes
         && tree_data.GetNumberOfChildren() == 0;
}

role_data_t ToRoleData(const tree_data_t &tree_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  if (!IsDataRoleConvertible(tree_data))
  {
    throw RuntimeException("Error in variant converter: invalid TreeData object");
  }

  const auto type_name = GetTypeName(tree_data);
  auto it = converters.find(type_name);
  if (it == converters.end())
  {
    throw RuntimeException("Error in variant converter: can't find type name [" + type_name + "]");
  }

  return it->second.treedata_to_roledata(tree_data);
}

tree_data_t ToTreeData(const role_data_t& role_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  const std::string type_name = utils::TypeName(role_data.second);

  auto it = converters.find(type_name);
  if (it == converters.end())
  {
    throw RuntimeException("Error in variant converter: can't find type name [" + type_name + "]");
  }
  return it->second.roledata_to_treedata(role_data);
}

// ----------------------------------------------------------------------------
// Implementation of helper functions from anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
// std::vector<std::string> GetExpectedAttributeKeys()
//{
//   std::vector<std::string> result = {kRoleAttributeKey, kTypeAttributeKey};
//   std::sort(result.begin(), result.end());
//   return result;
// }

int GetRole(const tree_data_t& tree_data)
{
  if (!tree_data.HasAttribute(kRoleAttributeKey))
  {
    throw mvvm::RuntimeException("Error in variant converter: absent [role] attribute");
  }
  return std::stoi(tree_data.GetAttribute(kRoleAttributeKey));
}

std::string GetTypeName(const mvvm::tree_data_t& tree_data)
{
  if (!tree_data.HasAttribute(kTypeAttributeKey))
  {
    throw mvvm::RuntimeException("Error in variant converter: absent [role] attribute");
  }
  return tree_data.GetAttribute(kTypeAttributeKey);
}

tree_data_t from_roledata_default_impl(const mvvm::role_data_t& role_data)
{
  tree_data_t result(kVariantElementType);
  result.AddAttribute(kTypeAttributeKey, mvvm::utils::TypeName(role_data.second));
  result.AddAttribute(kRoleAttributeKey, std::to_string(role_data.first));
  result.SetContent(mvvm::utils::ValueToString(role_data.second));
  return result;
}

mvvm::role_data_t to_undefined(const tree_data_t& tree_data)
{
  return {GetRole(tree_data), mvvm::variant_t()};
}

mvvm::role_data_t to_bool(const tree_data_t& tree_data)
{
  bool value = mvvm::utils::StringToBool(tree_data.GetContent());
  return {GetRole(tree_data), mvvm::variant_t(value)};
}

template <typename T>
mvvm::role_data_t to_int(const tree_data_t& tree_data)
{
  T value = std::stoi(tree_data.GetContent());
  return {GetRole(tree_data), mvvm::variant_t(value)};
}

mvvm::role_data_t to_string(const tree_data_t& tree_data)
{
  return {GetRole(tree_data), mvvm::variant_t(tree_data.GetContent())};
}

template <typename T>
mvvm::role_data_t to_double(const tree_data_t& tree_data)
{
  // we use own conversion to double instead of std::stod to not to depend on user's locale
  if (auto value = mvvm::utils::StringToDouble(tree_data.GetContent()); value)
  {
    T concrete_value(value.value());
    return {GetRole(tree_data), mvvm::variant_t(concrete_value)};
  }
  throw mvvm::RuntimeException("Error in variant converter: malformed double number");
}

mvvm::role_data_t to_vector_double(const tree_data_t& tree_data)
{
  auto values = mvvm::utils::ParseCommaSeparatedDoubles(tree_data.GetContent());
  return {GetRole(tree_data), mvvm::variant_t(values)};
}

tree_data_t from_combo_property(const role_data_t &role_data)
{
  tree_data_t result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(role_data.first));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kComboPropertyTypeName);
  auto combo = std::get<mvvm::ComboProperty>(role_data.second);
  result.AddAttribute(kSelectionsAttributeKey, combo.GetStringOfSelections());
  result.SetContent(combo.GetStringOfValues());
  return result;
}

role_data_t to_combo_property(const tree_data_t& tree_data)
{
  ComboProperty combo;
  combo.SetValuesFromString(tree_data.GetContent());
  auto selections = tree_data.GetAttribute(kSelectionsAttributeKey);
  if (!selections.empty())
  {
    combo.SetSelectionFromString(selections);
  }
  return {GetRole(tree_data), combo};
}

role_data_t to_external_property(const tree_data_t& tree_data)
{
  auto property = ExternalProperty::CreateFromString(tree_data.GetContent());
  return {GetRole(tree_data), property};
}

std::map<std::string, Converters> GetConverters()
{
  static std::map<std::string, Converters> result = {
      {constants::kEmptyTypeName, {from_roledata_default_impl, to_undefined}},
      {constants::kBooleanTypeName, {from_roledata_default_impl, to_bool}},
      {constants::kChar8TypeName, {from_roledata_default_impl, to_int<mvvm::char8>}},
      {constants::kInt8TypeName, {from_roledata_default_impl, to_int<mvvm::int8>}},
      {constants::kUInt8TypeName, {from_roledata_default_impl, to_int<mvvm::uint8>}},

      {constants::kInt16TypeName, {from_roledata_default_impl, to_int<mvvm::int16>}},
      {constants::kUInt16TypeName, {from_roledata_default_impl, to_int<mvvm::uint16>}},

      {constants::kInt32TypeName, {from_roledata_default_impl, to_int<mvvm::int32>}},
      {constants::kUInt32TypeName, {from_roledata_default_impl, to_int<mvvm::uint32>}},

      {constants::kInt64TypeName, {from_roledata_default_impl, to_int<mvvm::int64>}},
      {constants::kUInt64TypeName, {from_roledata_default_impl, to_int<mvvm::uint64>}},

      {constants::kStringTypeName, {from_roledata_default_impl, to_string}},
      {constants::kFloat32TypeName, {from_roledata_default_impl, to_double<mvvm::float32>}},
      {constants::kFloat64TypeName, {from_roledata_default_impl, to_double<mvvm::float64>}},
      {constants::kVectorDoubleTypeName, {from_roledata_default_impl, to_vector_double}},
      {constants::kComboPropertyTypeName, {from_combo_property, to_combo_property}},
      {constants::kExternalPropertyTypeName,
       {from_roledata_default_impl, to_external_property}}};

  return result;
}

}  // namespace

}  // namespace mvvm
