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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_FACTORY_H_
#define MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_FACTORY_H_

#include <mvvm/viewmodel/abstract_viewmodel_controller.h>
#include <mvvm/viewmodel_export.h>

#include <memory>

namespace mvvm
{

class IChildrenStrategy;
class IRowStrategy;
class AbstractViewModelController;
class ISessionModel;
class ViewModelBase;

namespace factory
{

struct ViewModelControllerFactoryContext
{
  std::unique_ptr<IChildrenStrategy> children_strategy;
  std::unique_ptr<IRowStrategy> row_strategy;
  ISessionModel* model{nullptr};
  ViewModelBase* view_model{nullptr};
};

/**
 * @brief Creates viewmodel controller.
 *
 * @param context The structure with parameters necessary for controller creation.
 */
std::unique_ptr<AbstractViewModelController> CreateViewModelController(
    ViewModelControllerFactoryContext&& context);

/**
 * @brief Creates default viewmodel controller.
 *
 * @tparam ChildrenStrategyT The strategy to look for children.
 * @taram RowStrategyT The strategy to construct the row.
 *
 * @param model The model to listen.
 * @param view_model The view model to update.
 */
template <typename ChildrenStrategyT, typename RowStrategyT>
std::unique_ptr<AbstractViewModelController> CreateController(ISessionModel* model,
                                                              ViewModelBase* view_model)
{
  return CreateViewModelController(
      {std::make_unique<ChildrenStrategyT>(), std::make_unique<RowStrategyT>(), model, view_model});
}

}  // namespace factory

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_FACTORY_H_
