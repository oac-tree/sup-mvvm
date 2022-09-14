/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/custom_editor_factories.h"

#include <mvvm/editors/custom_editor.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <iostream>

namespace
{

const mvvm::SessionItem* GetItemFromIndex(const QModelIndex& index)
{
  auto model = dynamic_cast<const mvvm::ViewModel*>(index.model());
  return model ? model->GetSessionItemFromIndex(index) : nullptr;
}

}  // namespace

namespace mvvm
{

// ----------------------------------------------------------------------------

RoleDependentEditorFactory::RoleDependentEditorFactory()
{
  RegisterBuilder(constants::kBoolEditorType, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyEditorType, ComboPropertyEditorBuilder());
  RegisterBuilder(constants::kSelectableComboPropertyEditorType,
                  SelectableComboPropertyEditorBuilder());
  RegisterBuilder(constants::kColorEditorType, ColorEditorBuilder());
  RegisterBuilder(constants::kExternalPropertyEditorType, ExternalPropertyEditorBuilder());
  RegisterBuilder(constants::kScientificSpinboxEditorType, ScientificSpinBoxEditorBuilder());
}

//! Creates cell editor basing on item role. It is expected that the index belongs to a ViewModel.

std::unique_ptr<CustomEditor> RoleDependentEditorFactory::CreateEditor(
    const QModelIndex& index) const
{
  auto item = GetItemFromIndex(index);
  return item ? CreateItemEditor(item) : std::unique_ptr<CustomEditor>();
}

//! Creates cell editor basing on editor type.

std::unique_ptr<CustomEditor> RoleDependentEditorFactory::CreateItemEditor(
    const SessionItem* item) const
{
  auto builder = FindBuilder(item->GetEditorType());
  return builder ? builder(item) : std::unique_ptr<CustomEditor>();
}

// ----------------------------------------------------------------------------

VariantDependentEditorFactory::VariantDependentEditorFactory()
{
  // registering set of builders for given variant names
  RegisterBuilder(constants::kBoolQtTypeName, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyQtTypeName, ComboPropertyEditorBuilder());
  RegisterBuilder(constants::kExternalPropertyQtTypeName, ExternalPropertyEditorBuilder());
}

//! Creates cell editor basing on variant name.

std::unique_ptr<CustomEditor> VariantDependentEditorFactory::CreateEditor(
    const QModelIndex& index) const
{
  auto builder = FindBuilder(utils::GetQtVariantName(index.data(Qt::EditRole)));
  return builder ? builder(GetItemFromIndex(index)) : std::unique_ptr<CustomEditor>();
}

// ----------------------------------------------------------------------------

DefaultEditorFactory::DefaultEditorFactory()
    : m_role_dependent_factory(std::make_unique<RoleDependentEditorFactory>())
    , m_variant_dependent_factory(std::make_unique<VariantDependentEditorFactory>())
{
}

//! Creates editor for given model index basing either on editorType() or specific variant name.

std::unique_ptr<CustomEditor> DefaultEditorFactory::CreateEditor(const QModelIndex& index) const
{
  // trying to created an editor basing on possibly defined EDITOR role
  auto editor = m_role_dependent_factory->CreateEditor(index);
  // if we do not succeed, then creating editor from variant type
  return editor ? std::move(editor) : m_variant_dependent_factory->CreateEditor(index);
}

}  // namespace mvvm
