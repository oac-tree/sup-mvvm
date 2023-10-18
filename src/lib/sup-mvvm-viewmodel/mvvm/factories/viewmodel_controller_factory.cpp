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

#include "viewmodel_controller_factory.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_virtual_parent_impl.h>

namespace mvvm::factory
{

std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy,
    std::unique_ptr<RowStrategyInterface> row_strategy, SessionModelInterface *model,
    ViewModelBase *view_model)
{
  auto impl = std::make_unique<ViewModelControllerVirtualParentImpl>(
      view_model, std::move(children_strategy), std::move(row_strategy));

  auto result = std::make_unique<ViewModelController>(std::move(impl));

  if (model)
  {
    result->SetModel(model);
  }

  return result;
}

}  // namespace mvvm::factory
