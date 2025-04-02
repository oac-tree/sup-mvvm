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

#ifndef MVVM_VIEWS_VIEWMODEL_COMPONENT_BUILDER_H_
#define MVVM_VIEWS_VIEWMODEL_COMPONENT_BUILDER_H_

//! @file
//! Various builder classes to simplify ViewModel configuration

#include <memory>

class QAbstractItemView;

namespace mvvm
{

class IEditorFactory;
class ICellDecorator;
class ViewModelDelegate;
class ViewModel;
class ItemViewComponentProvider;

/**
 * @brief The ViewModelDelegateBuilder is a builder to create ViewModelDelegate objects that require
 * complex wiring.
 */
class ViewModelDelegateBuilder
{
public:
  ~ViewModelDelegateBuilder();

  /**
   * @brief Creates cell editor factory and stores it in internal cache for further use.
   *
   * @tparam FactoryT The type of the factory.
   * @tparam Args Variadic arguments to construct a factory.
   * @param args Arguments to pass to the c-tor.
   * @return Returns self for fluent interface.
   */
  template <typename FactoryT, typename... Args>
  ViewModelDelegateBuilder& Factory(Args&&... args);

  /**
   * @brief Creates cell decorator and stores it in internal cache for further use.
   *
   * @tparam CellDecoratorT The type of the cell decorator.
   * @tparam Args Variadic arguments to construct cell decorator.
   * @param args Arguments to pass to the c-tor.
   * @return Returns self for fluent interface.
   */
  template <typename CellDecoratorT, typename... Args>
  ViewModelDelegateBuilder& Decorator(Args&&... args);

  /**
   * @brief Operator to create ViewModelDelegate via implicit convertion.
   *
   * We intentionally do not mark it "explicit".
   */
  operator std::unique_ptr<ViewModelDelegate>();

  /**
   * @brief Creates ViewModelDelegate and invalidates all caches.
   */
  std::unique_ptr<ViewModelDelegate> Build();

private:
  std::unique_ptr<IEditorFactory> m_editor_factory;
  std::unique_ptr<ICellDecorator> m_cell_decorator;
};

template <typename FactoryT, typename... Args>
ViewModelDelegateBuilder& ViewModelDelegateBuilder::Factory(Args&&... args)
{
  m_editor_factory = std::make_unique<FactoryT>(std::forward<Args>(args)...);
  return *this;
}

template <typename CellDecoratorT, typename... Args>
ViewModelDelegateBuilder& ViewModelDelegateBuilder::Decorator(Args&&... args)
{
  m_cell_decorator = std::make_unique<CellDecoratorT>(std::forward<Args>(args)...);
  return *this;
}

/**
 * @brief The ItemViewComponentProviderBuilder is a builder to create
 * ItemViewComponentProvider objects that require complex wiring.
 */
class ItemViewComponentProviderBuilder
{
public:
  ~ItemViewComponentProviderBuilder();

  /**
   * @brief Creates view model and stores it in internal cache for further use.
   *
   * @tparam ViewModelT The type of the factory.
   * @tparam Args Variadic arguments to construct a factory.
   * @param args Arguments to pass to the c-tor.
   * @return Returns self for fluent interface.
   */
  template <typename ViewModelT, typename... Args>
  ItemViewComponentProviderBuilder& ViewModel(Args&&... args);

  /**
   * @brief Creates view model and stores it in internal cache for further use.
   *
   * (convenience method when ViewModel have no c-tor parameters)
   *
   * @tparam ViewModelT The type of the factory.
   * @return Returns self for fluent interface.
   */
  template <typename ViewModelT>
  ItemViewComponentProviderBuilder& ViewModel();

  /**
   * @brief Returns a reference to internal builder to build delegates.
   */
  ViewModelDelegateBuilder& Delegate();

  /**
   * @brief Stores given pointer to QAbstractItemView in internal cache for further use.
   *
   * @param view The view which will be server by the provider.
   * @return Returns self for fluent interface.
   */
  ItemViewComponentProviderBuilder& View(QAbstractItemView* view);

  /**
   * @brief Operator to create ItemViewComponentProvider via implicit convertion.
   *
   * We intentionally do not mark it "explicit".
   */
  operator std::unique_ptr<ItemViewComponentProvider>();

  /**
   * @brief Creates cell editor factory and stores it in internal cache for further use.
   *
   * @tparam FactoryT The type of the factory.
   * @tparam Args Variadic arguments to construct a factory.
   * @param args Arguments to pass to the c-tor.
   * @return Returns self for fluent interface.
   */
  template <typename FactoryT, typename... Args>
  ItemViewComponentProviderBuilder& Factory(Args&&... args);

  /**
   * @brief Creates cell decorator and stores it in internal cache for further use.
   *
   * @tparam CellDecoratorT The type of the cell decorator.
   * @tparam Args Variadic arguments to construct cell decorator.
   * @param args Arguments to pass to the c-tor.
   * @return Returns self for fluent interface.
   */
  template <typename CellDecoratorT, typename... Args>
  ItemViewComponentProviderBuilder& Decorator(Args&&... args);

  /**
   * @brief Creates ItemViewComponentProvider and invalidates all caches.
   */
  std::unique_ptr<ItemViewComponentProvider> Build();

private:
  std::unique_ptr<mvvm::ViewModel> m_viewmodel;
  std::unique_ptr<mvvm::ViewModelDelegate> m_viewmodel_delegate;
  ViewModelDelegateBuilder m_viewmodel_delegate_builder;
  QAbstractItemView* m_view{nullptr};
};

template <typename ViewModelT, typename... Args>
ItemViewComponentProviderBuilder& ItemViewComponentProviderBuilder::ViewModel(Args&&... args)
{
  m_viewmodel = std::make_unique<ViewModelT>(std::forward<Args>(args)...);
  return *this;
}

template <typename ViewModelT>
ItemViewComponentProviderBuilder& ItemViewComponentProviderBuilder::ViewModel()
{
  // no model is defined yet, assuming it will be set later
  m_viewmodel = std::make_unique<ViewModelT>(/*model*/ nullptr);
  return *this;
}

template <typename FactoryT, typename... Args>
ItemViewComponentProviderBuilder& ItemViewComponentProviderBuilder::Factory(Args&&... args)
{
  Delegate().Factory<FactoryT>(std::forward<Args>(args)...);
  return *this;
}

template <typename CellDecoratorT, typename... Args>
ItemViewComponentProviderBuilder& ItemViewComponentProviderBuilder::Decorator(Args&&... args)
{
  Delegate().Decorator<CellDecoratorT>(std::forward<Args>(args)...);
  return *this;
}

/**
 * @brief Creates provider builder (and provide implicit conversion to
 * unique_ptr<ItemViewComponentProvider>)
 */
ItemViewComponentProviderBuilder CreateProvider();

}  // namespace mvvm

#endif  // MVVM_VIEWS_VIEWMODEL_DELEGATE_H_
