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

#ifndef MVVM_CORE_VARIANT_H_
#define MVVM_CORE_VARIANT_H_

//! @file
//! Defines all supported elementary data types.

#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>
#include <mvvm/model/limits.h>
#include <mvvm/model_export.h>

#include <string>
#include <variant>
#include <vector>

using variant_t = std::variant<std::monostate, bool, int, double, std::string, std::vector<double>,
                               mvvm::ComboProperty, mvvm::ExternalProperty, mvvm::Limits<int>,
                               mvvm::Limits<double>>;

using datarole_t = std::pair<variant_t, int>;
bool operator==(const datarole_t& lhs, const datarole_t& rhs);

//! Defines names of all supported (serializable) data types.
namespace mvvm::constants
{
const std::string kUndefinedVariantName = "undefined";
const std::string kBoolVariantName = "bool";
const std::string kIntVariantName = "int";
const std::string kStringVariantName = "string";
const std::string kDoubleVariantName = "double";
const std::string kVectorDoubleVariantName = "vector_double_t";
const std::string kComboPropertyVariantName = "ComboProperty";
const std::string kExternalPropertyVariantName = "ExternalProperty";
const std::string kRealLimitsVariantName = "RealLimits";
const std::string kIntLimitsVariantName = "IntLimits";
}  // namespace mvvm::constants

namespace mvvm::utils
{
//! Returns true if given variant is properly initialized (i.e. is not default-constructed).
MVVM_MODEL_EXPORT bool IsValid(const variant_t& value);

//! Returns true if two variants are compatible, i.e. we are alowed to replace one variant with
//! another.
MVVM_MODEL_EXPORT bool AreCompatible(const variant_t& var1, const variant_t& var2);

//! Returns string representing type name.
MVVM_MODEL_EXPORT std::string TypeName(const variant_t& variant);

}  // namespace mvvm::utils

#endif  // MVVM_CORE_VARIANT_H_
