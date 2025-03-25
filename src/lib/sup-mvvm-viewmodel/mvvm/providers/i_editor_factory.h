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

#ifndef MVVM_PROVIDERS_I_EDITOR_FACTORY_H_
#define MVVM_PROVIDERS_I_EDITOR_FACTORY_H_

#include <mvvm/providers/editor_types_fwd.h>

class QModelIndex;

namespace mvvm
{

/**
 * @brief The IEditorFactory class is an interface for a custom editor factory.
 *
 * Creates cell editors for Qt trees and tables from the model index. Cell editor is a Qt widget
 * intended to edit the DATA role of some SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT IEditorFactory
{
public:
  IEditorFactory() = default;
  virtual ~IEditorFactory() = default;

  IEditorFactory(const IEditorFactory&) = delete;
  IEditorFactory& operator=(const IEditorFactory&) = delete;
  IEditorFactory(IEditorFactory&&) = delete;
  IEditorFactory& operator=(IEditorFactory&&) = delete;

  /**
   * @brief Returns factory function to create an editor suitable for a cell with the given index.
   */
  virtual editor_t CreateEditor(const QModelIndex& index) const = 0;

  /**
   * @brief Returns factory function to create an editor suitable for editing given item.
   */
  virtual editor_t CreateEditor(const SessionItem* item) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_EDITOR_FACTORY_H_
