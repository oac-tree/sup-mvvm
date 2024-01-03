/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "variant_converter.h"

#include "custom_variants.h"

#include <mvvm/core/exceptions.h>

#include <QMetaType>
#include <functional>
#include <map>
#include <stdexcept>

namespace
{
using converter_func_t = std::function<mvvm::variant_t(const QVariant& variant)>;
std::map<std::string, converter_func_t> CreateConverterMap()
{
  std::map<std::string, converter_func_t> result = {
      {mvvm::constants::kEmptyQtTypeName,
       [](const QVariant& /*variant*/) { return mvvm::variant_t(); }},

      {mvvm::constants::kBooleanQtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toBool()); }},

      {mvvm::constants::kChar8QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::char8>()); }},

      {mvvm::constants::kInt8QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::int8>()); }},

      {mvvm::constants::kUInt8QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::uint8>()); }},

      {mvvm::constants::kInt16QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::int16>()); }},

      {mvvm::constants::kUInt16QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::uint16>()); }},

      {mvvm::constants::kInt32QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toInt()); }},

      {mvvm::constants::kUInt32QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toUInt()); }},

      {mvvm::constants::kInt64QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::int64>()); }},

      {mvvm::constants::kUInt64QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::uint64>()); }},

      {mvvm::constants::kLongLongQtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<mvvm::int64>()); }},

      {mvvm::constants::kFloat32QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toFloat()); }},

      {mvvm::constants::kFloat64QtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toDouble()); }},

      {mvvm::constants::kStringQtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.toString().toStdString()); }},

      {mvvm::constants::kStdStringQtTypeName,
       [](const QVariant& variant) { return mvvm::variant_t(variant.value<std::string>()); }},

      {mvvm::constants::kStdVectorDoubleQtTypeName, [](const QVariant& variant)
       { return mvvm::variant_t(variant.value<std::vector<double>>()); }},

      {mvvm::constants::kComboPropertyQtTypeName, [](const QVariant& variant)
       { return mvvm::variant_t(variant.value<mvvm::ComboProperty>()); }},

      {mvvm::constants::kExternalPropertyQtTypeName, [](const QVariant& variant)
       { return mvvm::variant_t(variant.value<mvvm::ExternalProperty>()); }}

  };
  return result;
};

}  // namespace

namespace mvvm
{
QVariant GetQtVariant(const variant_t& variant)
{
  if (utils::TypeName(variant) == constants::kVectorDoubleTypeName)
  {
    // shall we convert to supported QVector<double> instead?
    return QVariant::fromValue(std::get<std::vector<double>>(variant));
  }

  if (utils::TypeName(variant) == constants::kStringTypeName)
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
