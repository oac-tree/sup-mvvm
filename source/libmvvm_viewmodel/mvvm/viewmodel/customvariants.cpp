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

#include "mvvm/viewmodel/customvariants.h"

namespace mvvm::Utils
{
std::string GetQtVariantName(const QVariant& variant)
{
  return variant.isValid() ? variant.typeName() : Constants::kInvalidQtTypeName;
}

int GetQtVariantType(const QVariant& variant)
{
  auto result = static_cast<int>(variant.type());
  if (result == QVariant::UserType)
    result = variant.userType();
  return result;
}

bool IsBoolVariant(const QVariant& variant)
{
  return variant.type() == QVariant::Bool;
}

bool IsIntVariant(const QVariant& variant)
{
  return variant.type() == QVariant::Int;
}

bool IsDoubleVariant(const QVariant& variant)
{
  return variant.type() == QVariant::Double;
}

bool IsStdStringVariant(const QVariant& variant)
{
  return variant.typeName() == Constants::kStdStringQtTypeName;
}

bool IsDoubleVectorVariant(const QVariant& variant)
{
  return variant.typeName() == Constants::kStdVectorDoubleQtTypeName;
}

}  // namespace mvvm::Utils
