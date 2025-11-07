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

#ifndef MVVM_PROVIDERS_CUSTOM_VARIANTS_H_
#define MVVM_PROVIDERS_CUSTOM_VARIANTS_H_

//! @file customvariants.h
//! Custom Qt variants registrations and utility functions.

#include <mvvm/core/basic_scalar_types.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>

#include <QVariant>
#include <string>

namespace mvvm
{

namespace constants
{

constexpr std::string_view kEmptyQtTypeName = "invalid";
constexpr std::string_view kBooleanQtTypeName = "bool";
constexpr std::string_view kChar8QtTypeName = "char";
constexpr std::string_view kInt8QtTypeName = "signed char";
constexpr std::string_view kUInt8QtTypeName = "uchar";
constexpr std::string_view kInt16QtTypeName = "short";
constexpr std::string_view kUInt16QtTypeName = "ushort";
constexpr std::string_view kInt32QtTypeName = "int";
constexpr std::string_view kUInt32QtTypeName = "uint";
constexpr std::string_view kInt64QtTypeName = "long";
constexpr std::string_view kUInt64QtTypeName = "ulong";
constexpr std::string_view kFloat32QtTypeName = "float";
constexpr std::string_view kFloat64QtTypeName = "double";
constexpr std::string_view kStdVectorDoubleQtTypeName = "std::vector<double>";
constexpr std::string_view kComboPropertyQtTypeName = "mvvm::ComboProperty";
constexpr std::string_view kExternalPropertyQtTypeName = "mvvm::ExternalProperty";
constexpr std::string_view kLongLongQtTypeName = "qlonglong";
constexpr std::string_view kStringQtTypeName = "QString";
}  // namespace constants

namespace utils
{
//! Returns name of variant.
std::string GetQtVariantName(const QVariant& variant);

//! Returns type of variant (additionally checks for user type).
std::int32_t GetQtVariantType(const QVariant& variant);

//! Returns true if variant is based on bool.
bool IsBoolVariant(const QVariant& variant);

//! Returns true if variant is based on int.
bool IsIntVariant(const QVariant& variant);

//! Returns true if variant is based on double.
bool IsDoubleVariant(const QVariant& variant);

//! Returns true if variant is based on std::vector<double>.
bool IsDoubleVectorVariant(const QVariant& variant);

//! Returns true if variant is based on ComboProperty.
bool IsComboPropertyVariant(const QVariant& variant);

//! Returns true if variant is based on ExternalProperty.
bool IsExternalPropertyVariant(const QVariant& variant);

}  // namespace utils

}  // namespace mvvm

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(mvvm::ComboProperty)
Q_DECLARE_METATYPE(mvvm::ExternalProperty)

#endif  // MVVM_PROVIDERS_CUSTOM_VARIANTS_H_
