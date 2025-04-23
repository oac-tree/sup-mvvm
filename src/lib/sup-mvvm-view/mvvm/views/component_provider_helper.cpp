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

#include "component_provider_helper.h"

#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_delegate.h>
#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

std::unique_ptr<IEditorFactory> CreateDefaultCellEditorFactory()
{
  return std::make_unique<DefaultEditorFactory>();
}

std::unique_ptr<ICellDecorator> CreateDefaultCellDecorator()
{
  return std::make_unique<DefaultCellDecorator>();
}

std::unique_ptr<ViewModelDelegate> CreateDefaultViewModelDelegate()
{
  return std::make_unique<ViewModelDelegate>(CreateDefaultCellEditorFactory(),
                                             CreateDefaultCellDecorator());
}

}  // namespace mvvm
