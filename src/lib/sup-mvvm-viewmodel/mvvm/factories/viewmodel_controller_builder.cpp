/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/factories/viewmodel_controller_builder.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_virtual_parent_impl.h>

#include <stdexcept>

namespace mvvm
{

ViewModelControllerBuilder::ViewModelControllerBuilder() = default;

ViewModelControllerBuilder::~ViewModelControllerBuilder() = default;

ViewModelControllerBuilder::operator std::unique_ptr<AbstractViewModelController>()
{
  if (!m_context.children_strategy)
  {
    throw std::runtime_error("Error in ViewModelControllerBuilder: no children strategy defined.");
  }

  if (!m_context.row_strategy)
  {
    throw std::runtime_error("Error in ViewModelControllerBuilder: no row strategy defined.");
  }

  if (m_context.model && !m_context.model->GetEventHandler())
  {
    throw std::runtime_error("Model doesn't have an event handler.");
  }
  
  auto controller_impl = std::make_unique<ViewModelControllerVirtualParentImpl>(
      m_context.view_model, std::move(m_context.children_strategy),
      std::move(m_context.row_strategy));
  auto result = std::make_unique<ViewModelController>(std::move(controller_impl));
  if (m_context.model)
  {
    result->SetModel(m_context.model);
  }

  return result;
}

ViewModelControllerBuilder::self& ViewModelControllerBuilder::SetModel(SessionModelInterface* model)
{
  m_context.model = model;
  return *this;
}

ViewModelControllerBuilder::self& ViewModelControllerBuilder::SetViewModel(
    ViewModelBase* view_model)
{
  m_context.view_model = view_model;
  return *this;
}

ViewModelControllerBuilder::self& ViewModelControllerBuilder::SetChildrenStrategy(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy)
{
  m_context.children_strategy = std::move(children_strategy);
  return *this;
}

ViewModelControllerBuilder::self& ViewModelControllerBuilder::SetRowStrategy(
    std::unique_ptr<RowStrategyInterface> row_strategy)
{
  m_context.row_strategy = std::move(row_strategy);
  return *this;
}

}  // namespace mvvm
