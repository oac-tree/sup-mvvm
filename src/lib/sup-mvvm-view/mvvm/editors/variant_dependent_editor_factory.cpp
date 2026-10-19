/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "variant_dependent_editor_factory.h"

#include "editor_builders.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/custom_variants.h>
#include <mvvm/providers/variant_converter.h>
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/standarditems/editor_constants.h>

#include <QWidget>

namespace mvvm
{

namespace
{

/**
 * @brief Returns Qt variant name corresponding to the data stored on board of item.
 */
std::string GetQtVariantName(const SessionItem* item, role_t role = DataRole::kData)
{
  if (item == nullptr)
  {
    return {};
  }

  auto qt_variant = GetQtVariant(item->Data(role));
  return utils::GetQtVariantName(qt_variant);
}

/**
 * @brief Returns the name of the builder for the item with the given Qt variant name of its data.
 *
 * @details A nullable item without data uses the type of its default value. A nullable int32 uses
 * AllIntSpinBox, since QSpinBox can't be empty.
 */
std::string GetBuilderName(const std::string& qt_variant_name, const SessionItem* item)
{
  if ((item == nullptr) || !item->HasFlag(Appearance::kNullable))
  {
    return qt_variant_name;
  }

  const auto result = (qt_variant_name == constants::kEmptyQtTypeName)
                          ? GetQtVariantName(item, DataRole::kDefaultValue)
                          : qt_variant_name;

  return (result == constants::kInt32QtTypeName) ? std::string(constants::kAllIntSpinBoxEditorType)
                                                 : result;
}

}  // namespace

VariantDependentEditorFactory::VariantDependentEditorFactory()
{
  // registering default builders for given variant names

  // we keep editing int32 numbers with QSpinBox to be able to compare the appearance with the rest
  RegisterBuilder(constants::kInt32QtTypeName, IntegerEditorBuilder());

  // new AllIntSpinBox for all other integer-like types
  RegisterBuilder(constants::kChar8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt16QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt16QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt32QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt64QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt64QtTypeName, AllIntSpinBoxEditorBuilder());

  // nullable int32
  RegisterBuilder(constants::kAllIntSpinBoxEditorType, AllIntSpinBoxEditorBuilder());

  // new FloatSpinBox editor for float32 and float64
  RegisterBuilder(constants::kFloat32QtTypeName, FloatEditorBuilder());
  RegisterBuilder(constants::kFloat64QtTypeName, FloatEditorBuilder());

  // other custom editors
  RegisterBuilder(constants::kBooleanQtTypeName, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyQtTypeName, ComboPropertyEditorBuilder());
  RegisterBuilder(constants::kExternalPropertyQtTypeName, ExternalPropertyEditorBuilder());
}

editor_t VariantDependentEditorFactory::CreateEditor(const QModelIndex& index) const
{
  auto item = utils::ItemFromProxyIndex(index);
  auto builder =
      FindBuilder(GetBuilderName(utils::GetQtVariantName(index.data(Qt::EditRole)), item));
  return builder ? builder(item) : editor_t{};
}

editor_t VariantDependentEditorFactory::CreateEditor(const SessionItem* item) const
{
  auto builder = FindBuilder(GetBuilderName(GetQtVariantName(item), item));
  return builder ? builder(item) : editor_t{};
}

}  // namespace mvvm
