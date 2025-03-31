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

#include "viewmodel_component_builder.h"

#include <mvvm/providers/i_cell_decorator.h>
#include <mvvm/providers/i_editor_factory.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_delegate.h>

namespace mvvm
{

ViewModelDelegateBuilder::operator std::unique_ptr<ViewModelDelegate>()
{
  if (!m_cell_decorator)
  {
    m_cell_decorator = std::make_unique<DefaultCellDecorator>();
  }

  return std::make_unique<ViewModelDelegate>(std::move(m_editor_factory),
                                             std::move(m_cell_decorator));
}

}  // namespace mvvm
