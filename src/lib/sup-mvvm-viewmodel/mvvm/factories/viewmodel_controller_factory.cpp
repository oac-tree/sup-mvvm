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

#include "viewmodel_controller_factory.h"

#include <mvvm/viewmodel/i_children_strategy.h>
#include <mvvm/viewmodel/i_row_strategy.h>
#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_impl.h>
#include <mvvm/viewmodel/viewmodel_controller_virtual_parent_impl.h>

namespace mvvm::factory
{

std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    ViewModelControllerFactoryContext &&context)
{
  std::unique_ptr<AbstractViewModelController> result;

  if (context.virtual_parent)
  {
    auto impl = std::make_unique<ViewModelControllerVirtualParentImpl>(
        context.view_model, std::move(context.children_strategy), std::move(context.row_strategy));
    result = std::make_unique<ViewModelController>(std::move(impl));
  }
  else
  {
    auto impl = std::make_unique<ViewModelControllerImpl>(
        context.view_model, std::move(context.children_strategy), std::move(context.row_strategy));
    result = std::make_unique<ViewModelController>(std::move(impl));
  }

  if (context.model)
  {
    result->SetModel(context.model);
  }

  return result;
}

}  // namespace mvvm::factory
