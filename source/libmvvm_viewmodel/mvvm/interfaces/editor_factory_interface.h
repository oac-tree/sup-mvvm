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

#ifndef MVVM_INTERFACES_EDITOR_FACTORY_INTERFACE_H_
#define MVVM_INTERFACES_EDITOR_FACTORY_INTERFACE_H_

#include <mvvm/viewmodel_export.h>

#include <memory>
#include <string>

class QModelIndex;
class QWidget;

namespace mvvm
{

class CustomEditor;

//! Interface for custom editor factory.
//! Intended for editor construction in cells of tables and trees in the context of delegate.

class MVVM_VIEWMODEL_EXPORT EditorFactoryInterface
{
public:
  virtual ~EditorFactoryInterface() = default;

  virtual std::unique_ptr<CustomEditor> CreateEditor(const QModelIndex& index) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_EDITOR_FACTORY_INTERFACE_H_
