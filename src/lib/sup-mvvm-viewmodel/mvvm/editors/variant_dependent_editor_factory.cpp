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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "variant_dependent_editor_factory.h"

#include "editor_builders.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/viewmodel/variant_converter.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QWidget>

namespace mvvm
{

namespace
{

/**
 * @brief Returns Qt variant name corresponding to the data stored on board of item.
 */
std::string GetQtVariantName(const SessionItem* item, int role = DataRole::kData)
{
  if (!item)
  {
    return {};
  }

  auto qt_variant = GetQtVariant(item->Data(role));
  return utils::GetQtVariantName(qt_variant);
}

}  // namespace

VariantDependentEditorFactory::VariantDependentEditorFactory()
{
  // registering default builders for given variant names

  // we keep editing int32 numbers with QSpinBox to be able to compare the appearance with the rest
  RegisterBuilder(constants::kInt32QtTypeName, IntegerEditorBuilder());

  // new AllIntSpinBoxEditor for all other integer-like types
  RegisterBuilder(constants::kChar8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt8QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt16QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt16QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt32QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kInt64QtTypeName, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kUInt64QtTypeName, AllIntSpinBoxEditorBuilder());

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
  auto builder = FindBuilder(utils::GetQtVariantName(index.data(Qt::EditRole)));
  return builder ? builder(utils::ItemFromProxyIndex(index)) : editor_t{};
}

editor_t VariantDependentEditorFactory::CreateEditor(const SessionItem* item) const
{
  auto builder = FindBuilder(GetQtVariantName(item));
  return builder ? builder(item) : editor_t{};
}

}  // namespace mvvm
