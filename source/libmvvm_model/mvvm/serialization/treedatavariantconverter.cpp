// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedatavariantconverter.h"

#include "mvvm/serialization/TreeData.h"
#include "mvvm/utils/stringutils.h"

#include <functional>
#include <stdexcept>

// ----------------------------------------------------------------------------
// Declarations of constants and helper methods in anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
const std::string kTaggedItemsElementType = "Variant";
const std::string kRoleAttributeKey = "role";
const std::string kTypeAttributeKey = "type";

//! Aggregates call backs for convertion between datarole_t and TreeData.
struct Converters
{
  std::function<ModelView::TreeData(const datarole_t& datarole)> datarole_to_treedata;
  std::function<datarole_t(const ModelView::TreeData& treedata)> treedata_to_datarole;
};

//! Returns vector of attributes which TreeData object should have.
std::vector<std::string> GetExpectedAttributeKeys();

//! Returns role attribute.
int GetRole(const ModelView::TreeData& tree_data);

//! Returns variant type name.
std::string GetTypeName(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding undefined data to the TreeData object.
ModelView::TreeData from_undefined(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding undefined data.
datarole_t to_undefined(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding boolean to the TreeData object.
ModelView::TreeData from_bool(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding boolean.
datarole_t to_bool(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding integer data to the TreeData object.
ModelView::TreeData from_int(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding integer data.
datarole_t to_int(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding string data to the TreeData object.
ModelView::TreeData from_string(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding string data.
datarole_t to_string(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding double to the TreeData object.
ModelView::TreeData from_double(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding double data.
datarole_t to_double(const ModelView::TreeData& tree_data);

//! Converts datarole_t holding vector<doubl>e to the TreeData object.
ModelView::TreeData from_vector_double(const datarole_t& datarole);

//! Converts TreeData to datarole_t holding vector<double> data.
datarole_t to_vector_double(const ModelView::TreeData& tree_data);

//! Returns map of all defined converters.
std::map<std::string, Converters> GetConverters();

}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

namespace ModelView
{
bool IsDataRoleConvertible(const TreeData& tree_data)
{
  static const std::vector<std::string> expected_names = GetExpectedAttributeKeys();
  return tree_data.GetType() == kTaggedItemsElementType
         && expected_names == tree_data.Attributes().GetAttributeNames()
         && tree_data.GetNumberOfChildren() == 0;
}

datarole_t ToDataRole(const TreeData& tree_data)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  if (!IsDataRoleConvertible(tree_data))
    throw std::runtime_error("Error in variant converter: invalid TreeData object.");

  const auto type_name = GetTypeName(tree_data);
  auto it = converters.find(type_name);
  if (it == converters.end())
    throw std::runtime_error("Error in variant converter: can't find type name '" + type_name
                             + "'");

  return it->second.treedata_to_datarole(tree_data);
}

TreeData ToTreeData(const datarole_t& data_role)
{
  static const std::map<std::string, Converters> converters = GetConverters();

  const std::string type_name = Utils::TypeName(data_role.first);

  auto it = converters.find(type_name);
  if (it == converters.end())
    throw std::runtime_error("Error in variant converter: can't find type name '" + type_name
                             + "'");
  return it->second.datarole_to_treedata(data_role);
}

}  // namespace ModelView

// ----------------------------------------------------------------------------
// Implementation of helper functions from anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
std::vector<std::string> GetExpectedAttributeKeys()
{
  std::vector<std::string> result = {kRoleAttributeKey, kTypeAttributeKey};
  std::sort(result.begin(), result.end());
  return result;
}

int GetRole(const ModelView::TreeData& tree_data)
{
  if (!tree_data.HasAttribute(kRoleAttributeKey))
    throw std::runtime_error("Error in variant converter: absent 'role' attribute.");
  return std::stoi(tree_data.GetAttribute(kRoleAttributeKey));
}

std::string GetTypeName(const ModelView::TreeData& tree_data)
{
  if (!tree_data.HasAttribute(kTypeAttributeKey))
    throw std::runtime_error("Error in variant converter: absent 'role' attribute.");
  return tree_data.GetAttribute(kTypeAttributeKey);
}

ModelView::TreeData from_undefined(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kUndefinedTypeName);
  return result;
}

datarole_t to_undefined(const ModelView::TreeData& tree_data)
{
  return datarole_t{variant_t(), GetRole(tree_data)};
}

ModelView::TreeData from_bool(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kBoolTypeName);
  bool value = std::get<bool>(datarole.first);
  result.SetContent(ModelView::Utils::FromBool(value));
  return result;
}

datarole_t to_bool(const ModelView::TreeData& tree_data)
{
  bool value = ModelView::Utils::StringToBool(tree_data.GetContent());
  return datarole_t{variant_t(value), GetRole(tree_data)};
}

ModelView::TreeData from_int(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kIntTypeName);
  auto value = std::get<int>(datarole.first);
  result.SetContent(std::to_string(value));
  return result;
}

datarole_t to_int(const ModelView::TreeData& tree_data)
{
  int value = std::stoi(tree_data.GetContent());
  return datarole_t{variant_t(value), GetRole(tree_data)};
}

ModelView::TreeData from_string(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kStringTypeName);
  auto value = std::get<std::string>(datarole.first);
  result.SetContent(value);
  return result;
}

datarole_t to_string(const ModelView::TreeData& tree_data)
{
  return datarole_t{variant_t(tree_data.GetContent()), GetRole(tree_data)};
}

ModelView::TreeData from_double(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kDoubleTypeName);
  auto value = std::get<double>(datarole.first);
  result.SetContent(ModelView::Utils::DoubleToString(value));
  return result;
}

datarole_t to_double(const ModelView::TreeData& tree_data)
{
  // we use own conversion to double instead of std::stod to not to depend on user's locale
  if (auto value = ModelView::Utils::StringToDouble(tree_data.GetContent()); value)
    return datarole_t{variant_t(value.value()), GetRole(tree_data)};
  throw std::runtime_error("Error in variant converter: malformed double number.");
}

ModelView::TreeData from_vector_double(const datarole_t& datarole)
{
  ModelView::TreeData result(kTaggedItemsElementType);
  result.AddAttribute(kRoleAttributeKey, std::to_string(datarole.second));
  result.AddAttribute(kTypeAttributeKey, ModelView::Constants::kVectorDoubleTypeName);
  auto values = std::get<std::vector<double>>(datarole.first);
  result.SetContent(ModelView::Utils::ToCommaSeparatedString(values));
  return result;
}

datarole_t to_vector_double(const ModelView::TreeData& tree_data)
{
  auto values = ModelView::Utils::ParseCommaSeparatedDoubles(tree_data.GetContent());
  return datarole_t{variant_t(values), GetRole(tree_data)};
}

std::map<std::string, Converters> GetConverters()
{
  static std::map<std::string, Converters> result = {
      {ModelView::Constants::kUndefinedTypeName, {from_undefined, to_undefined}},
      {ModelView::Constants::kBoolTypeName, {from_bool, to_bool}},
      {ModelView::Constants::kIntTypeName, {from_int, to_int}},
      {ModelView::Constants::kStringTypeName, {from_string, to_string}},
      {ModelView::Constants::kDoubleTypeName, {from_double, to_double}},
      {ModelView::Constants::kVectorDoubleTypeName, {from_vector_double, to_vector_double}}};

  return result;
}

}  // namespace
