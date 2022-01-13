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

#include "mvvm/viewmodel/variantconverter.h"

#include "mvvm/viewmodel/customvariants.h"

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
      {mvvm::Constants::kInvalidQtTypeName,
       [](const QVariant& /*variant*/) { return variant_t(); }},
      {mvvm::Constants::kBoolQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toBool()); }},
      {mvvm::Constants::kIntQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toInt()); }},
      {mvvm::Constants::kDoubleQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toDouble()); }},
      {mvvm::Constants::kStringQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.toString().toStdString()); }},
      {mvvm::Constants::kStdVectorDoubleQtTypeName,
       [](const QVariant& variant) { return variant_t(variant.value<std::vector<double>>()); }}};

  return result;
}

}  // namespace

namespace mvvm
{
QVariant GetQtVariant(const variant_t& variant)
{
  if (Utils::TypeName(variant) == Constants::kVectorDoubleTypeName)
  {
    // shall we convert to supported QVector<double> instead?
    return QVariant::fromValue(std::get<std::vector<double>>(variant));
  }
  else if (Utils::TypeName(variant) == Constants::kStringTypeName)
  {
    //  converting std::string to QString
    return QVariant::fromValue(QString::fromStdString(std::get<std::string>(variant)));
  }
  else
  {
    return QVariant::fromStdVariant(variant);
  }
}

variant_t GetStdVariant(const QVariant& variant)
{
  static auto converter_map = CreateConverterMap();
  auto it = converter_map.find(Utils::GetQtVariantName(variant));
  if (it == converter_map.end())
    throw std::runtime_error("Unsupported Qt variant");
  return it->second(variant);
}

}  // namespace ModelView
