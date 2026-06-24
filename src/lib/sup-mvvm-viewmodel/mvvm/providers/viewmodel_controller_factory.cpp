/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "viewmodel_controller_factory.h"

#include "i_children_strategy.h"
#include "i_row_strategy.h"
#include "viewmodel_controller.h"

namespace mvvm::factory
{

std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    ViewModelControllerFactoryContext&& context)
{
  auto result = std::make_unique<ViewModelController>(
      context.view_model, std::move(context.children_strategy), std::move(context.row_strategy));

  if (context.model != nullptr)
  {
    result->SetModel(context.model);
  }

  return result;
}

}  // namespace mvvm::factory
