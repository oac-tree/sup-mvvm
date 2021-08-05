// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/core/variant.h"

#include <map>

bool operator==(const datarole_t &lhs, const datarole_t &rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool ModelView::Utils::IsValid(const variant_t &value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

bool ModelView::Utils::AreCompatible(const variant_t &var1, const variant_t &var2)
{
  // If one of them is invalid, they are considered to be compatible.
  if (!IsValid(var1) || !IsValid(var2))
    return true;

  // However, properly initialized variants of different types are considered to be incompatible.
  return var1.index() == var2.index();
}

std::string ModelView::Utils::TypeName(const variant_t &variant)
{
  static std::map<int, std::string> type_name_map = {
      {0, Constants::kUndefinedTypeName}, {1, Constants::kBoolTypeName},
      {2, Constants::kIntTypeName},       {3, Constants::kDoubleTypeName},
      {4, Constants::kStringTypeName},    {5, Constants::kVectorDoubleTypeName}};
  return type_name_map[static_cast<int>(variant.index())];
}
