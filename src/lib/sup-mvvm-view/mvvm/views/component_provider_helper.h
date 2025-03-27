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

#ifndef MVVM_VIEWS_COMPONENT_PROVIDER_HELPER_H_
#define MVVM_VIEWS_COMPONENT_PROVIDER_HELPER_H_

#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/item_view_component_provider.h>

namespace mvvm
{

class IEditorFactory;
class ICellDecorator;
class ViewModelDelegate;

/**
 * @brief Creates default factory to edit cells of Qt's trees and tables.
 */
std::unique_ptr<IEditorFactory> CreateDefaultCellEditorFactory();

/**
 * @brief Creates default decorator for cells of Qt's trees and tables.
 */
std::unique_ptr<ICellDecorator> CreateDefaultCellDecorator();

/**
 * @brief Creates default delegate for cells of Qt's trees and tables.
 */
std::unique_ptr<ViewModelDelegate> CreateDefaultViewModelDelegate();

/**
 * @brief Creates viewmodel delegate for given decoration types.
 *
 * @tparam FactoryT The type of cell editor factory.
 * @tparam DecoratorT The type of cell decorator.
 */
template <typename FactoryT, typename DecoratorT = DefaultCellDecorator>
std::unique_ptr<ViewModelDelegate> CreateViewModelDelegate()
{
  return std::make_unique<ViewModelDelegate>(std::make_unique<FactoryT>(),
                                             std::make_unique<DecoratorT>());
}

/**
 * @brief Convenience function to create component provider for a view using given viewmodel type.
 *
 * It will create delegate and viewmodel and store them on board of provider.
 *
 * @tparam ViewModelT The type of ViewModel.
 * @param view The view which will be served by the provider.
 * @param model The model which will be used to setup viewmodel.
 *
 * @return Component provider.
 */
template <typename ViewModelT>
std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                          ISessionModel* model = nullptr)
{
  return std::make_unique<ItemViewComponentProvider>(std::make_unique<ViewModelT>(model), view);
}

/**
 * @brief Convenience function to create component provider for a view.
 *
 * It will create delegate and viewmodel and store them on board of provider.
 *
 * @tparam DelegateT The type of the delegate.
 * @tparam ViewModelT The type of ViewModel.
 * @param view The view which will be served by the provider.
 * @param model The model which will be used to setup viewmodel.
 *
 * @return Component provider.
 */
template <typename DelegateT, typename ViewModelT>
std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                          ISessionModel* model = nullptr)
{
  return std::make_unique<ItemViewComponentProvider>(std::make_unique<DelegateT>(model),
                                                     std::make_unique<ViewModelT>(model), view);
}

}  // namespace mvvm

#endif  // MVVM_VIEWS_COMPONENT_PROVIDER_HELPER_H_
