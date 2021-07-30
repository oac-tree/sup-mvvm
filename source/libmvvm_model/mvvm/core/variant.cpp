// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/core/variant.h"

#include <map>

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
      {0, Constants::undefined_type_name}, {1, Constants::bool_type_name},
      {2, Constants::int_type_name},       {3, Constants::double_type_name},
      {4, Constants::string_type_name},    {5, Constants::vector_double_type_name}};
  return type_name_map[static_cast<int>(variant.index())];
}
