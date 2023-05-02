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

#include "mvvm/viewmodel/variant_converter.h"

#include <mvvm/viewmodel/custom_variants.h>

#include <QMetaType>
#include <functional>
#include <map>
#include <stdexcept>

namespace
{
using converter_func_t = std::function<variant_t(const QVariant& variant)>;
std::map<std::string, converter_func_t> CreateConverterMap()
{
  std::map<std::string, converter_func_t> result = {
      {mvvm::constants::kInvalidQtTypeName,
       [](const QVariant& /*variant*/) { return variant_t(); }},
      {mvvm::constants::kBoolQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toBool()); }},
      {mvvm::constants::kIntQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toInt()); }},
      {mvvm::constants::kDoubleQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toDouble()); }},
      {mvvm::constants::kStringQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toString().toStdString()); }},
      {mvvm::constants::kStdVectorDoubleQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<std::vector<double>>()); }},
      {mvvm::constants::kComboPropertyQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<mvvm::ComboProperty>()); }},
      {mvvm::constants::kExternalPropertyQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<mvvm::ExternalProperty>()); }},
      {mvvm::constants::kIntLimitsPropertyQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<mvvm::Limits<int>>()); }},
      {mvvm::constants::kRealLimitsPropertyQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<mvvm::Limits<double>>()); }}};
  return result;
};

}  // namespace

namespace mvvm
{
QVariant GetQtVariant(const variant_t& variant)
{
  if (utils::TypeName(variant) == constants::kVectorDoubleVariantName)
  {
    // shall we convert to supported QVector<double> instead?
    return QVariant::fromValue(std::get<std::vector<double>>(variant));
  }

  if (utils::TypeName(variant) == constants::kStringVariantName)
  {
    //  converting std::string to QString
    return QVariant::fromValue(QString::fromStdString(std::get<std::string>(variant)));
  }

  return QVariant::fromStdVariant(variant);
}

variant_t GetStdVariant(const QVariant& variant)
{
  static auto converter_map = CreateConverterMap();
  auto it = converter_map.find(utils::GetQtVariantName(variant));
  if (it == converter_map.end())
  {
    throw std::runtime_error("Unsupported Qt variant");
  }
  return it->second(variant);
}

}  // namespace mvvm
