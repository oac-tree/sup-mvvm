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

#include "mvvm/serialization/treedata_variant_converter.h"

#include <mvvm/serialization/limits_utils.h>
#include <mvvm/serialization/treedata.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_utils.h>

#include <functional>
#include <stdexcept>

// ----------------------------------------------------------------------------
// Declarations of constants and helper methods in anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
const std::string kVariantElementType = "Variant";
const std::string kRoleAttributeKey = "role";
const std::string kTypeAttributeKey = "type";
const std::string kSelectionsAttributeKey = "selections";

//! Aggregates call backs for convertion between datarole_t and TreeData.
struct Converters
{
  std::function<mvvm::TreeData(const mvvm::datarole_t& datarole)> datarole_to_treedata;
  std::function<mvvm::datarole_t(const mvvm::TreeData& treedata)> treedata_to_datarole;
};

////! Returns vector of attributes which TreeData object should have.
// std::vector<std::string> GetExpectedAttributeKeys();

//! Returns role attribute.
int GetRole(const mvvm::TreeData& tree_data);

//! Returns variant type name.
std::string GetTypeName(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding undefined data to the TreeData object.
mvvm::TreeData from_undefined(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding undefined data.
mvvm::datarole_t to_undefined(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding boolean to the TreeData object.
mvvm::TreeData from_bool(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding boolean.
mvvm::datarole_t to_bool(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding integer data to the TreeData object.
mvvm::TreeData from_int(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding integer data.
mvvm::datarole_t to_int(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding string data to the TreeData object.
mvvm::TreeData from_string(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding string data.
mvvm::datarole_t to_string(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding double to the TreeData object.
mvvm::TreeData from_double(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding double data.
mvvm::datarole_t to_double(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding vector<double> to the TreeData object.
mvvm::TreeData from_vector_double(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding vector<double>.
mvvm::datarole_t to_vector_double(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding ComboProperty to the TreeData object.
mvvm::TreeData from_combo_property(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding ComboProperty data.
mvvm::datarole_t to_combo_property(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding ExternalProperty to the TreeData object.
mvvm::TreeData from_external_property(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding ExternalProperty data.
mvvm::datarole_t to_external_property(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding Limits<int> to the TreeData object.
mvvm::TreeData from_int_limits(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding Limits<int> data.
mvvm::datarole_t to_int_limits(const mvvm::TreeData& tree_data);

//! Converts datarole_t holding Limits<double> to the TreeData object.
mvvm::TreeData from_real_limits(const mvvm::datarole_t& datarole);

//! Converts TreeData to datarole_t holding Limits<double> data.
mvvm::datarole_t to_real_limits(const mvvm::TreeData& tree_data);

//! Returns map of all defined converters.
std::map<std::string, Converters> GetConverters();

}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

namespace mvvm
{
bool IsDataRoleConvertible(const TreeData& tree_data)
{
  auto attribute_names = tree_data.Attributes().GetAttributeNames();
  const bool correct_attributes = utils::Contains(attribute_names, kRoleAttributeKey)
                                  && utils::Contains(attribute_names, kTypeAttributeKey);
  return tree_data.GetType() == kVariantElementType && correct_attributes
         && tree_data.GetNumberOfChildren() == 0;
}

datarole_t ToDataRole(const TreeData& tree_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  if (!IsDataRoleConvertible(tree_data))
  {
    throw std::runtime_error("Error in variant converter: invalid TreeData object.");
  }

  const auto type_name = GetTypeName(tree_data);
  auto it = converters.find(type_name);
  if (it == converters.end())
  {
    throw std::runtime_error("Error in variant converter: can't find type name '" + type_name
                             + "'");
  }

  return it->second.treedata_to_datarole(tree_data);
}

TreeData ToTreeData(const datarole_t& data_role)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  const std::string type_name = utils::TypeName(data_role.first);

  auto it = converters.find(type_name);
  if (it == converters.end())
  {
    throw std::runtime_error("Error in variant converter: can't find type name '" + type_name
                             + "'");
  }
  return it->second.datarole_to_treedata(data_role);
}

}  // namespace mvvm

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

int GetRole(const mvvm::TreeData& tree_data)
{
  if (!tree_data.HasAttribute(kRoleAttributeKey))
  {
    throw std::runtime_error("Error in variant converter: absent 'role' attribute.");
  }
  return std::stoi(tree_data.GetAttribute(kRoleAttributeKey));
}

std::string GetTypeName(const mvvm::TreeData& tree_data)
{
  if (!tree_data.HasAttribute(kTypeAttributeKey))
  {
    throw std::runtime_error("Error in variant converter: absent 'role' attribute.");
  }
  return tree_data.GetAttribute(kTypeAttributeKey);
}

mvvm::TreeData from_undefined(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kEmptyTypeName);
  return result;
}

mvvm::datarole_t to_undefined(const mvvm::TreeData& tree_data)
{
  return {mvvm::variant_t(), GetRole(tree_data)};
}

mvvm::TreeData from_bool(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kBooleanTypeName);
  bool value = std::get<bool>(datarole.first);
  result.SetContent(mvvm::utils::FromBool(value));
  return result;
}

mvvm::datarole_t to_bool(const mvvm::TreeData& tree_data)
{
  bool value = mvvm::utils::StringToBool(tree_data.GetContent());
  return {mvvm::variant_t(value), GetRole(tree_data)};
}

mvvm::TreeData from_int(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kInt64TypeName);
  auto value = std::get<mvvm::int64>(datarole.first);
  result.SetContent(std::to_string(value));
  return result;
}

mvvm::datarole_t to_int(const mvvm::TreeData& tree_data)
{
  int value = std::stoi(tree_data.GetContent());
  return {mvvm::variant_t(value), GetRole(tree_data)};
}

mvvm::TreeData from_string(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kStringTypeName);
  auto value = std::get<std::string>(datarole.first);
  result.SetContent(value);
  return result;
}

mvvm::datarole_t to_string(const mvvm::TreeData& tree_data)
{
  return {mvvm::variant_t(tree_data.GetContent()), GetRole(tree_data)};
}

mvvm::TreeData from_double(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kFloat64TypeName);
  auto value = std::get<double>(datarole.first);
  result.SetContent(mvvm::utils::DoubleToString(value));
  return result;
}

mvvm::datarole_t to_double(const mvvm::TreeData& tree_data)
{
  // we use own conversion to double instead of std::stod to not to depend on user's locale
  if (auto value = mvvm::utils::StringToDouble(tree_data.GetContent()); value)
  {
    return {mvvm::variant_t(value.value()), GetRole(tree_data)};
  }
  throw std::runtime_error("Error in variant converter: malformed double number.");
}

mvvm::TreeData from_vector_double(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kVectorDoubleVariantName);
  auto values = std::get<std::vector<double>>(datarole.first);
  result.SetContent(mvvm::utils::ToCommaSeparatedString(values));
  return result;
}

mvvm::datarole_t to_vector_double(const mvvm::TreeData& tree_data)
{
  auto values = mvvm::utils::ParseCommaSeparatedDoubles(tree_data.GetContent());
  return {mvvm::variant_t(values), GetRole(tree_data)};
}

mvvm::TreeData from_combo_property(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kComboPropertyVariantName);
  auto combo = std::get<mvvm::ComboProperty>(datarole.first);
  result.AddAttribute(kSelectionsAttributeKey, combo.GetStringOfSelections());
  result.SetContent(combo.GetStringOfValues());
  return result;
}

mvvm::datarole_t to_combo_property(const mvvm::TreeData& tree_data)
{
  mvvm::ComboProperty combo;
  combo.SetStringOfValues(tree_data.GetContent());
  auto selections = tree_data.GetAttribute(kSelectionsAttributeKey);
  if (!selections.empty())
  {
    combo.SetStringOfSelections(selections);
  }
  return {combo, GetRole(tree_data)};
}

mvvm::TreeData from_external_property(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kExternalPropertyVariantName);
  auto property = std::get<mvvm::ExternalProperty>(datarole.first);
  result.SetContent(property.ToString());
  return result;
}

mvvm::datarole_t to_external_property(const mvvm::TreeData& tree_data)
{
  auto property = mvvm::ExternalProperty::CreateFromString(tree_data.GetContent());
  return {property, GetRole(tree_data)};
}

mvvm::TreeData from_int_limits(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kIntLimitsVariantName);
  auto limits = std::get<mvvm::Limits<int>>(datarole.first);
  result.SetContent(mvvm::utils::ToString(limits));
  return result;
}

mvvm::datarole_t to_int_limits(const mvvm::TreeData& tree_data)
{
  auto limits = mvvm::utils::IntLimitsFromString(tree_data.GetContent());
  return {limits, GetRole(tree_data)};
}

mvvm::TreeData from_real_limits(const mvvm::datarole_t& datarole)
{
  mvvm::TreeData result(kVariantElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, mvvm::constants::kRealLimitsVariantName);
  auto limits = std::get<mvvm::Limits<double>>(datarole.first);
  result.SetContent(mvvm::utils::ToString(limits));
  return result;
}

mvvm::datarole_t to_real_limits(const mvvm::TreeData& tree_data)
{
  auto limits = mvvm::utils::RealLimitsFromString(tree_data.GetContent());
  return {limits, GetRole(tree_data)};
}

std::map<std::string, Converters> GetConverters()
{
  static std::map<std::string, Converters> result = {
      {mvvm::constants::kEmptyTypeName, {from_undefined, to_undefined}},
      {mvvm::constants::kBooleanTypeName, {from_bool, to_bool}},
      {mvvm::constants::kInt64TypeName, {from_int, to_int}},
      {mvvm::constants::kStringTypeName, {from_string, to_string}},
      {mvvm::constants::kFloat64TypeName, {from_double, to_double}},
      {mvvm::constants::kVectorDoubleVariantName, {from_vector_double, to_vector_double}},
      {mvvm::constants::kComboPropertyVariantName, {from_combo_property, to_combo_property}},
      {mvvm::constants::kExternalPropertyVariantName,
       {from_external_property, to_external_property}},
      {mvvm::constants::kIntLimitsVariantName, {from_int_limits, to_int_limits}},
      {mvvm::constants::kRealLimitsVariantName, {from_real_limits, to_real_limits}},
  };

  return result;
}

}  // namespace
