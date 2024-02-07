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

#include "custom_editor_factories.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QWidget>

namespace mvvm
{

// ----------------------------------------------------------------------------

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

//! Creates cell editor basing on item role. It is expected that the index belongs to a ViewModel.

editor_t RoleDependentEditorFactory::CreateEditor(const QModelIndex& index) const
{
  auto item = utils::ItemFromIndex(index);
  return item ? CreateItemEditor(item) : editor_t{};
}

//! Creates cell editor basing on editor type.

editor_t RoleDependentEditorFactory::CreateItemEditor(const SessionItem* item) const
{
  auto builder = FindBuilder(item->GetEditorType());
  return builder ? builder(item) : editor_t{};
}

// ----------------------------------------------------------------------------

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

//! Creates cell editor basing on variant name.

editor_t VariantDependentEditorFactory::CreateEditor(const QModelIndex& index) const
{
  auto builder = FindBuilder(utils::GetQtVariantName(index.data(Qt::EditRole)));
  return builder ? builder(utils::ItemFromIndex(index)) : editor_t{};
}

// ----------------------------------------------------------------------------

DefaultEditorFactory::DefaultEditorFactory()
    : m_role_dependent_factory(std::make_unique<RoleDependentEditorFactory>())
    , m_variant_dependent_factory(std::make_unique<VariantDependentEditorFactory>())
{
}

//! Creates editor for given model index basing either on editorType() or specific variant name.

editor_t DefaultEditorFactory::CreateEditor(const QModelIndex& index) const
{
  // trying to created an editor basing on possibly defined DataRole::kEditor role
  auto editor = m_role_dependent_factory->CreateEditor(index);
  // if we do not succeed, then creating editor from variant type
  return editor ? std::move(editor) : m_variant_dependent_factory->CreateEditor(index);
}

}  // namespace mvvm
