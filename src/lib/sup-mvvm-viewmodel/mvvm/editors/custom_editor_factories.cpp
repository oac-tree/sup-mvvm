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

#include "custom_editor_factories.h"

#include "editor_builders.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/viewmodel/variant_converter.h>
#include <mvvm/viewmodel/viewmodel.h>
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

// -------------------------------------------------------------------------------------------------
// RoleDependentEditorFactory
// -------------------------------------------------------------------------------------------------
RoleDependentEditorFactory::RoleDependentEditorFactory()
{
  // registering all existing builders under their names

  RegisterBuilder(constants::kBoolEditorType, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyEditorType, ComboPropertyEditorBuilder());
  RegisterBuilder(constants::kSelectableComboPropertyEditorType,
                  SelectableComboPropertyEditorBuilder());
  RegisterBuilder(constants::kColorEditorType, ColorEditorBuilder());
  RegisterBuilder(constants::kExternalPropertyEditorType, ExternalPropertyEditorBuilder());
  RegisterBuilder(constants::kIntegerEditorType, IntegerEditorBuilder());
  RegisterBuilder(constants::kScientificSpinboxEditorType, ScientificSpinBoxEditorBuilder());
  RegisterBuilder(constants::kDoubleEditorType, DoubleEditorBuilder());
  RegisterBuilder(constants::kAllIntSpinBoxEditorType, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kFloatSpinBoxEditorType, FloatEditorBuilder());
}

editor_t RoleDependentEditorFactory::CreateEditor(const QModelIndex& index) const
{
  auto item = utils::ItemFromProxyIndex(index);
  return item ? CreateEditor(item) : editor_t{};
}

editor_t RoleDependentEditorFactory::CreateEditor(const SessionItem* item) const
{
  auto builder = FindBuilder(item->GetEditorType());
  return builder ? builder(item) : editor_t{};
}

// -------------------------------------------------------------------------------------------------
// VariantDependentEditorFactory
// -------------------------------------------------------------------------------------------------
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

// -------------------------------------------------------------------------------------------------
// DefaultEditorFactory
// -------------------------------------------------------------------------------------------------
DefaultEditorFactory::DefaultEditorFactory()
    : m_role_dependent_factory(std::make_unique<RoleDependentEditorFactory>())
    , m_variant_dependent_factory(std::make_unique<VariantDependentEditorFactory>())
{
}

editor_t DefaultEditorFactory::CreateEditor(const QModelIndex& index) const
{
  // trying to created an editor basing on possibly defined DataRole::kEditor role
  auto editor = m_role_dependent_factory->CreateEditor(index);
  // if we do not succeed, then creating editor from variant type
  return editor ? std::move(editor) : m_variant_dependent_factory->CreateEditor(index);
}

editor_t DefaultEditorFactory::CreateEditor(const SessionItem* item) const
{
  // trying to created an editor basing on possibly defined DataRole::kEditor role
  auto editor = m_role_dependent_factory->CreateEditor(item);
  // if we do not succeed, then creating editor from variant type
  return editor ? std::move(editor) : m_variant_dependent_factory->CreateEditor(item);
}

}  // namespace mvvm
