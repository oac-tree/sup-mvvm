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

#ifndef MVVM_EDITORS_VARIANT_DEPENDENT_EDITOR_FACTORY_H_
#define MVVM_EDITORS_VARIANT_DEPENDENT_EDITOR_FACTORY_H_

#include <mvvm/providers/abstract_editor_factory.h>

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

}  // namespace mvvm

#endif  // MVVM_EDITORS_VARIANT_DEPENDENT_EDITOR_FACTORY_H_
