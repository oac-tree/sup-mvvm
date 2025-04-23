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

#ifndef MVVM_VIEWMODEL_CUSTOM_VARIANTS_H_
#define MVVM_VIEWMODEL_CUSTOM_VARIANTS_H_

//! @file customvariants.h
//! Custom Qt variants registrations and utility functions.

#include <mvvm/core/basic_scalar_types.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>
#include <mvvm/viewmodel_export.h>

#include <QVariant>
#include <string>

namespace mvvm
{

namespace constants
{

const std::string kEmptyQtTypeName = "invalid";
const std::string kBooleanQtTypeName = "bool";
const std::string kChar8QtTypeName = "char";
const std::string kInt8QtTypeName = "signed char";
const std::string kUInt8QtTypeName = "uchar";
const std::string kInt16QtTypeName = "short";
const std::string kUInt16QtTypeName = "ushort";
const std::string kInt32QtTypeName = "int";
const std::string kUInt32QtTypeName = "uint";
const std::string kInt64QtTypeName = "long";
const std::string kUInt64QtTypeName = "ulong";
const std::string kFloat32QtTypeName = "float";
const std::string kFloat64QtTypeName = "double";
const std::string kStdVectorDoubleQtTypeName = "std::vector<double>";
const std::string kComboPropertyQtTypeName = "mvvm::ComboProperty";
const std::string kExternalPropertyQtTypeName = "mvvm::ExternalProperty";
const std::string kLongLongQtTypeName = "qlonglong";
const std::string kStringQtTypeName = "QString";
}  // namespace constants

namespace utils
{
//! Returns name of variant.
MVVM_VIEWMODEL_EXPORT std::string GetQtVariantName(const QVariant& variant);

//! Returns type of variant (additionally checks for user type).
MVVM_VIEWMODEL_EXPORT int GetQtVariantType(const QVariant& variant);

//! Returns true if variant is based on bool.
MVVM_VIEWMODEL_EXPORT bool IsBoolVariant(const QVariant& variant);

//! Returns true if variant is based on int.
MVVM_VIEWMODEL_EXPORT bool IsIntVariant(const QVariant& variant);

//! Returns true if variant is based on double.
MVVM_VIEWMODEL_EXPORT bool IsDoubleVariant(const QVariant& variant);

//! Returns true if variant is based on std::vector<double>.
MVVM_VIEWMODEL_EXPORT bool IsDoubleVectorVariant(const QVariant& variant);

//! Returns true if variant is based on ComboProperty.
MVVM_VIEWMODEL_EXPORT bool IsComboPropertyVariant(const QVariant& variant);

//! Returns true if variant is based on ExternalProperty.
MVVM_VIEWMODEL_EXPORT bool IsExternalPropertyVariant(const QVariant& variant);

}  // namespace utils

}  // namespace mvvm

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(mvvm::ComboProperty)
Q_DECLARE_METATYPE(mvvm::ExternalProperty)

#endif  // MVVM_VIEWMODEL_CUSTOM_VARIANTS_H_
