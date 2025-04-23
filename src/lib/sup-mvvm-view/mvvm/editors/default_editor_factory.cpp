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

#include "default_editor_factory.h"

#include "role_dependent_editor_factory.h"
#include "variant_dependent_editor_factory.h"

#include <QWidget>

namespace mvvm
{

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
