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

namespace ModelView::Utils
{
//! Returns true if given variant is properly initialized (i.e. is not default-constructed).
bool IsValid(const variant_t& value);

//! Returns true if two variants are compatible, i.e. we are alowed to replace one variant with
//! another.
bool AreCompatible(const variant_t& var1, const variant_t& var2);

}  // namespace ModelView::Utils

#endif  // MVVM_CORE_VARIANT_H
