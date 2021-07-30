// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_CORE_VARIANT_H
#define MVVM_CORE_VARIANT_H

//! @file variant.h
//! Defines all supported elementary data types.

#include <string>
#include <variant>
#include <vector>

using variant_t = std::variant<std::monostate, bool, int, double, std::string, std::vector<double>>;

namespace ModelView::Constants
{
const std::string undefined_type_name = "undefined";
const std::string bool_type_name = "bool";
const std::string int_type_name = "int";
const std::string string_type_name = "std::string";
const std::string double_type_name = "double";
const std::string vector_double_type_name = "std::vector<double>";
}  // namespace ModelView::Constants

namespace ModelView::Utils
{
//! Returns true if given variant is properly initialized (i.e. is not default-constructed).
bool IsValid(const variant_t& value);

//! Returns true if two variants are compatible, i.e. we are alowed to replace one variant with
//! another.
bool AreCompatible(const variant_t& var1, const variant_t& var2);

//! Returns string representing type name.
std::string TypeName(const variant_t& variant);

}  // namespace ModelView::Utils

#endif  // MVVM_CORE_VARIANT_H
