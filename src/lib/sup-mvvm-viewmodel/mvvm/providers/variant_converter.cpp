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

#include "variant_converter.h"

#include "custom_variants.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <QMetaType>
#include <functional>
#include <map>

namespace mvvm
{

namespace
{
using converter_func_t = std::function<variant_t(const QVariant& variant)>;
std::map<std::string, converter_func_t> CreateConverterMap()
{
  std::map<std::string, converter_func_t> result = {
      {std::string(constants::kEmptyQtTypeName),
       [](const QVariant& /*variant*/) { return variant_t(); }},

      {std::string(constants::kBooleanQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toBool()); }},

      {std::string(constants::kChar8QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<char8>()); }},

      {std::string(constants::kInt8QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<int8>()); }},

      {std::string(constants::kUInt8QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<uint8>()); }},

      {std::string(constants::kInt16QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<int16>()); }},

      {std::string(constants::kUInt16QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<uint16>()); }},

      {std::string(constants::kInt32QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toInt()); }},

      {std::string(constants::kUInt32QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toUInt()); }},

      {std::string(constants::kInt64QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<int64>()); }},

      {std::string(constants::kUInt64QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<uint64>()); }},

      {std::string(constants::kLongLongQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<int64>()); }},

      {std::string(constants::kFloat32QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toFloat()); }},

      {std::string(constants::kFloat64QtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toDouble()); }},

      {std::string(constants::kStringQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.toString().toStdString()); }},

      {std::string(constants::kStdVectorDoubleQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<std::vector<double>>()); }},

      {std::string(constants::kComboPropertyQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<ComboProperty>()); }},

      {std::string(constants::kExternalPropertyQtTypeName),
       [](const QVariant& variant) { return variant_t(variant.value<ExternalProperty>()); }}

  };
  return result;
}

}  // namespace

QVariant GetQtVariant(const variant_t& variant)
{
  if (utils::TypeName(variant) == constants::VectorDoubleTypeName())
  {
    // shall we convert to supported QVector<double> instead?
    return QVariant::fromValue(std::get<std::vector<double>>(variant));
  }

  if (utils::TypeName(variant) == constants::StringTypeName())
  {
    //  converting std::string to QString
    return QVariant::fromValue(QString::fromStdString(std::get<std::string>(variant)));
  }

  return QVariant::fromStdVariant(variant);
}

variant_t GetStdVariant(const QVariant& variant)
{
  const static auto converter_map = CreateConverterMap();
  auto iter = converter_map.find(utils::GetQtVariantName(variant));
  if (iter == converter_map.end())
  {
    throw RuntimeException("Unsupported Qt variant");
  }
  return iter->second(variant);
}

}  // namespace mvvm
