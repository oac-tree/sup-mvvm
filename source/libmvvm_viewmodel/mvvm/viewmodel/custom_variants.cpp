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

#include "mvvm/viewmodel/custom_variants.h"

namespace mvvm::utils
{
std::string GetQtVariantName(const QVariant& variant)
{
  return variant.isValid() ? variant.typeName() : constants::kInvalidQtTypeName;
}

int GetQtVariantType(const QVariant& variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  auto result = static_cast<int>(variant.type());
  if (result == QVariant::UserType)
  {
    result = variant.userType();
  }
#else
  auto result = variant.typeId();
  if (result == QMetaType::User)
  {
    result = variant.userType();
  }
#endif

  return result;
}

bool IsBoolVariant(const QVariant& variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  return variant.type() == QVariant::Bool;
#else
  return variant.typeId() == QMetaType::Bool;
#endif
}

bool IsIntVariant(const QVariant& variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  return variant.type() == QVariant::Int;
#else
  return variant.typeId() == QMetaType::Int;
#endif
}

bool IsDoubleVariant(const QVariant& variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  return variant.type() == QVariant::Double;
#else
  return variant.typeId() == QMetaType::Double;
#endif
}

bool IsStdStringVariant(const QVariant& variant)
{
  return variant.typeName() == constants::kStdStringQtTypeName;
}

bool IsDoubleVectorVariant(const QVariant& variant)
{
  return variant.typeName() == constants::kStdVectorDoubleQtTypeName;
}

bool IsComboPropertyVariant(const QVariant& variant)
{
  return variant.typeName() == constants::kComboPropertyQtTypeName;
}

bool IsExternalPropertyVariant(const QVariant& variant)
{
  return variant.typeName() == constants::kExternalPropertyQtTypeName;
}

bool IsIntLimitsVariant(const QVariant &variant)
{
  return variant.typeName() == constants::kIntLimitsPropertyQtTypeName;
}

bool IsRealLimitsVariant(const QVariant &variant)
{
  return variant.typeName() == constants::kRealLimitsPropertyQtTypeName;
}

}  // namespace mvvm::utils
