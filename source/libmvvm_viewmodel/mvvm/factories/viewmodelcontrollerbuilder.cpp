/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/factories/viewmodelcontrollerbuilder.h"

#include "mvvm/interfaces/childrenstrategyinterface.h"
#include "mvvm/interfaces/rowstrategyinterface.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/viewmodelcontroller.h"

#include <stdexcept>

namespace mvvm
{

ViewModelControllerBuilder::ViewModelControllerBuilder() = default;

ViewModelControllerBuilder::~ViewModelControllerBuilder() = default;

ViewModelControllerBuilder::operator std::unique_ptr<AbstractViewModelController>()
{
  if (!m_context.model)
  {
    throw std::runtime_error("Error in ViewModelController: undefined model");
  }

  if (!m_context.children_strategy)
  {
    throw std::runtime_error("Error in ViewModelController: no children strategy defined.");
  }

  if (!m_context.row_strategy)
  {
    throw std::runtime_error("Error in ViewModelController: no row strategy defined.");
  }

  auto result = std::make_unique<ViewModelController>(m_context.model, m_context.view_model);
  result->SubscribeTo(m_context.model->GetSubscriber());
  result->SetChildrenStrategy(std::move(m_context.children_strategy));
  result->SetRowStrategy(std::move(m_context.row_strategy));

  return result;
}

ViewModelControllerBuilder::self& ViewModelControllerBuilder::SetModel(ApplicationModel* model)
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
