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

namespace mvvm
{

class IEditorFactory;
class ICellDecorator;
class ViewModelDelegate;

/**
 * @brief The ViewModelDelegateBuilder class builds complex ViewModelDelegate.
 */
class ViewModelDelegateBuilder
{
public:
  /**
   * @brief Creates cell editor factory and stores it in internal cache for futher use.
   *
   * @tparam FactoryT The type of the factory.
   * @tparam Args Variadic arguments to construct a factory.
   * @param args Arguments that factory needs
   * @return Returns self for fluent interface.
   */
  template <typename FactoryT, typename... Args>
  ViewModelDelegateBuilder& Factory(Args&&... args);

  /**
   * @brief Creates cell decorator and stores it in internal cache for futher use.
   *
   * @tparam FactoryT The type of the factory.
   * @tparam Args Variadic arguments to construct a factory.
   * @param args Arguments that factory needs
   * @return Returns self for fluent interface.
   */
  template <typename FactoryT, typename... Args>
  ViewModelDelegateBuilder& Decorator(Args&&... args);

  /**
   * @brief Operator to create ViewModelDelegate.
   *
   * We do not mark it "explicit" intentionally.
   */
  operator std::unique_ptr<ViewModelDelegate>();

private:
  std::unique_ptr<IEditorFactory> m_editor_factory;
  std::unique_ptr<ICellDecorator> m_cell_decorator;
};

template <typename C, typename... Args>
ViewModelDelegateBuilder& ViewModelDelegateBuilder::Factory(Args&&... args)
{
  m_editor_factory = std::make_unique<C>(std::forward<Args>(args)...);
  return *this;
}

template <typename C, typename... Args>
ViewModelDelegateBuilder& ViewModelDelegateBuilder::Decorator(Args&&... args)
{
  m_cell_decorator = std::make_unique<C>(std::forward<Args>(args)...);
  return *this;
}

}  // namespace mvvm

#endif  // MVVM_VIEWS_VIEWMODEL_DELEGATE_H_
