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

#include "mvvm/editors/customeditorfactories.h"

#include "mvvm/editors/customeditor.h"
#include "mvvm/editors/editor_constants.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/customvariants.h"
#include "mvvm/viewmodel/viewmodel.h"

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
  RegisterBuilder(constants::BoolEditorType, BoolEditorBuilder());
  RegisterBuilder(constants::ComboPropertyEditorType, ComboPropertyEditorBuilder());
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
  // FIXME !!!
  //  auto builder = findBuilder(item->editorType());
  //  return builder ? builder(item) : std::unique_ptr<CustomEditor>();
  return {};
}

// ----------------------------------------------------------------------------

VariantDependentEditorFactory::VariantDependentEditorFactory()
{
  // registering set of builders for given variant names
  RegisterBuilder(constants::kBoolQtTypeName, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyQtTypeName, ComboPropertyEditorBuilder());
}

//! Creates cell editor basing on variant name.

std::unique_ptr<CustomEditor> VariantDependentEditorFactory::CreateEditor(
    const QModelIndex& index) const
{
  //  auto item = GetItemFromIndex(index);
  //  auto value = item ? item->data<QVariant>() : index.data(Qt::EditRole);
  //  auto builder = findBuilder(utils::GetQtVariantName(value));
  //  return builder ? builder(item) : std::unique_ptr<CustomEditor>();
  return {};
}

// ----------------------------------------------------------------------------

DefaultEditorFactory::DefaultEditorFactory()
    : m_roleDependentFactory(std::make_unique<RoleDependentEditorFactory>())
    , m_variantDependentFactory(std::make_unique<VariantDependentEditorFactory>())
{
}

//! Creates editor for given model index basing either on editorType() or specific variant name.

std::unique_ptr<CustomEditor> DefaultEditorFactory::CreateEditor(const QModelIndex& index) const
{
  // trying to created an editor basing on possibly defined EDITOR role
  auto editor = m_roleDependentFactory->CreateEditor(index);
  // if we do not succeed, then creating editor from variant type
  return editor ? std::move(editor) : m_variantDependentFactory->CreateEditor(index);
}

}  // namespace mvvm
