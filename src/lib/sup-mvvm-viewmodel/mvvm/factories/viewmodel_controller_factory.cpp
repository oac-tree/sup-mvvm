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

#include "viewmodel_controller_builder.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>

namespace mvvm::factory
{

std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy,
    std::unique_ptr<RowStrategyInterface> row_strategy, SessionModelInterface *model,
    ViewModelBase *view_model)
{
  return mvvm::ViewModelControllerBuilder()
      .SetModel(model)
      .SetViewModel(view_model)
      .SetChildrenStrategy(std::move(children_strategy))
      .SetRowStrategy(std::move(row_strategy));
}

}  // namespace mvvm::factory
