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

#ifndef MVVM_FACTORIES_VIEWMODEL_CONTROLLER_FACTORY_H_
#define MVVM_FACTORIES_VIEWMODEL_CONTROLLER_FACTORY_H_

#include <mvvm/viewmodel/abstract_viewmodel_controller.h>
#include <mvvm/viewmodel_export.h>

#include <memory>

namespace mvvm
{

class ChildrenStrategyInterface;
class RowStrategyInterface;
class AbstractViewModelController;
class SessionModelInterface;
class ViewModelBase;

namespace factory
{

/**
 * @brief Creates viewmodel controller.
 *
 * @param children_strategy The strategy to look for children.
 * @param row_strategy The strategy to construct the row.
 * @param model The model to listen.
 * @param view_model The view model to update.
 */
std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy,
    std::unique_ptr<RowStrategyInterface> row_strategy, SessionModelInterface* model,
    ViewModelBase* view_model);

//! Creates controller for view model using given strategies.

/**
 * @brief Creates viewmodel controller.
 *
 * @tparam ChildrenStrategyT The strategy to look for children.
 * @taram RowStrategyT The strategy to construct the row.
 *
 * @param model The model to listen.
 * @param view_model The view model to update.
 */
template <typename ChildrenStrategyT, typename RowStrategyT>
std::unique_ptr<AbstractViewModelController> CreateController(SessionModelInterface* model,
                                                              ViewModelBase* view_model)
{
  return CreateViewModelController(std::make_unique<ChildrenStrategyT>(),
                                   std::make_unique<RowStrategyT>(), model, view_model);
}

}  // namespace factory

}  // namespace mvvm

#endif  // MVVM_FACTORIES_VIEWMODEL_CONTROLLER_FACTORY_H_
