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

#include "property_viewmodel_controller.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"

#include <memory>

namespace mvvm
{

PropertyViewModelController::PropertyViewModelController(ISessionModel* model,
                                                         ViewModelBase* view_model)
    : ViewModelController(view_model, std::make_unique<PropertyItemsStrategy>(),
                          std::make_unique<LabelDataRowStrategy>())
{
  if (model != nullptr)
  {
    SetModel(model);
  }
}

}  // namespace mvvm
