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

#ifndef MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_
#define MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_

//! @file
//! Defines custom cell editor factories for Qt trees and tables.

#include <mvvm/providers/abstract_editor_factory.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The VariantDependentEditorFactory class constructs cell editors using the data role.
 *
 * The type of the editor will be deduced from the type of the variant stored for DataRole::kData
 * role on board of SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT VariantDependentEditorFactory : public AbstractEditorFactory
{
public:
  VariantDependentEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;

  editor_t CreateEditor(const SessionItem* item) const override;
};

/**
 * @brief The DefaultEditorFactory class is a default editor factory for cell editors in Qt trees
 * and tables.
 *
 * It first tries to construct a cell editor using the editor role, and then, in the case of
 * failure, switches to the data role.
 */
class MVVM_VIEWMODEL_EXPORT DefaultEditorFactory : public IEditorFactory
{
public:
  DefaultEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;

  editor_t CreateEditor(const SessionItem* item) const override;

private:
  std::unique_ptr<AbstractEditorFactory> m_role_dependent_factory;
  std::unique_ptr<AbstractEditorFactory> m_variant_dependent_factory;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_
