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

#ifndef MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_
#define MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_

//! @file
//! Defines DefaultEditorFactory and auxiliary classes for custom view model delegates.

#include <mvvm/providers/abstract_editor_factory.h>

#include <memory>

namespace mvvm
{

//! Editor factory for cell editors in Qt trees and tables, relies on DataRole::kEditor role stored
//! on board of SessionItem.

class MVVM_VIEWMODEL_EXPORT RoleDependentEditorFactory : public AbstractEditorFactory
{
public:
  RoleDependentEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;

protected:
  editor_t CreateItemEditor(const SessionItem* item) const;
};

//! Editor factory for cell editors in Qt trees and tables, relies on variant type stored as
//! DataRole::kData role on board of SessionItem.

class MVVM_VIEWMODEL_EXPORT VariantDependentEditorFactory : public AbstractEditorFactory
{
public:
  VariantDependentEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;
};

//! Default editor factory for cell editors in Qt trees and tables.
//! Internaly it uses two factories

class MVVM_VIEWMODEL_EXPORT DefaultEditorFactory : public IEditorFactory
{
public:
  DefaultEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;

private:
  std::unique_ptr<RoleDependentEditorFactory> m_role_dependent_factory;
  std::unique_ptr<VariantDependentEditorFactory> m_variant_dependent_factory;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_CUSTOM_EDITOR_FACTORIES_H_
