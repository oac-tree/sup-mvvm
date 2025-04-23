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

#ifndef MVVM_EDITORS_ROLE_DEPENDENT_EDITOR_FACTORY_H_
#define MVVM_EDITORS_ROLE_DEPENDENT_EDITOR_FACTORY_H_

#include <mvvm/providers/abstract_editor_factory.h>

namespace mvvm
{

/**
 * @brief The RoleDependentEditorFactory class constructs cell editors using the editor role.
 *
 * The user shall set the editor type via SessionItem::SetEditorType call.
 */
class MVVM_VIEWMODEL_EXPORT RoleDependentEditorFactory : public AbstractEditorFactory
{
public:
  RoleDependentEditorFactory();

  editor_t CreateEditor(const QModelIndex& index) const override;

  editor_t CreateEditor(const SessionItem* item) const override;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ROLE_DEPENDENT_EDITOR_FACTORY_H_
