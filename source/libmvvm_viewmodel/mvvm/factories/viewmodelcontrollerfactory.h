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

#ifndef MVVM_FACTORIES_VIEWMODELCONTROLLERFACTORY_H
#define MVVM_FACTORIES_VIEWMODELCONTROLLERFACTORY_H

#include "mvvm/factories/viewmodelcontrollerbuilder.h"
#include "mvvm/viewmodel_export.h"

#include <memory>

namespace mvvm
{

class ViewModelController;

namespace factory
{

//! Create universal controller.

template <typename ChildrenStrategy, typename RowStrategy>
std::unique_ptr<AbstractViewModelController> CreateController(ApplicationModel* model,
                                                              ViewModelBase* view_model)
{
  return ViewModelControllerBuilder()
      .SetModel(model)
      .SetViewModel(view_model)
      .SetChildrenStrategy(std::make_unique<ChildrenStrategy>())
      .SetRowStrategy(std::make_unique<RowStrategy>());
}

}  // namespace factory

}  // namespace mvvm

#endif  // MVVM_FACTORIES_VIEWMODELCONTROLLERFACTORY_H
