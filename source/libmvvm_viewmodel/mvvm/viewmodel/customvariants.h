/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_CUSTOMVARIANTS_H
#define MVVM_VIEWMODEL_CUSTOMVARIANTS_H

//! @file customvariants.h
//! Custom Qt variants registrations and utility functions.

#include "mvvm/viewmodel_export.h"

#include "mvvm/model/comboproperty.h"
#include "mvvm/model/externalproperty.h"

#include <QVariant>
#include <string>

namespace mvvm
{
namespace constants
{
const std::string kInvalidQtTypeName = "invalid";
const std::string kBoolQtTypeName = "bool";
const std::string kIntQtTypeName = "int";
const std::string kStringQtTypeName = "QString";
const std::string kStdStringQtTypeName = "std::string";
const std::string kDoubleQtTypeName = "double";
const std::string kStdVectorDoubleQtTypeName = "std::vector<double>";
const std::string kComboPropertyQtTypeName = "mvvm::ComboProperty";
const std::string kExternalPropertyQtTypeName = "mvvm::ExternalProperty";
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

//! Returns true if variant is based on std::string.
MVVM_VIEWMODEL_EXPORT bool IsStdStringVariant(const QVariant& variant);

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

#endif  // MVVM_VIEWMODEL_CUSTOMVARIANTS_H
