/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_FACTORIES_VIEWMODELCONTROLLERBUILDER_H
#define MVVM_FACTORIES_VIEWMODELCONTROLLERBUILDER_H

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"
#include "mvvm/viewmodel_export.h"

#include <memory>

namespace mvvm
{

class ApplicationModel;
class ViewModelBase;
class ChildrenStrategyInterface;
class RowStrategyInterface;

//! Builder class for ViewModelController.

class MVVM_VIEWMODEL_EXPORT ViewModelControllerBuilder
{
public:
  using self = ViewModelControllerBuilder;

  ViewModelControllerBuilder();
  ~ViewModelControllerBuilder();

  ViewModelControllerBuilder(const ViewModelControllerBuilder& other) = delete;
  ViewModelControllerBuilder& operator=(const ViewModelControllerBuilder& other) = delete;

  self& SetModel(ApplicationModel* model);
  self& SetViewModel(ViewModelBase* view_model);
  self& SetChildrenStrategy(std::unique_ptr<ChildrenStrategyInterface> children_strategy);
  self& SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy);

  operator std::unique_ptr<AbstractViewModelController>();

private:
  //! Components necessary to build ViewModelController
  struct Context
  {
    ApplicationModel* model{nullptr};
    ViewModelBase* view_model{nullptr};
    std::unique_ptr<ChildrenStrategyInterface> children_strategy;
    std::unique_ptr<RowStrategyInterface> row_strategy;
  };

  Context m_context;
};

}  // namespace mvvm

#endif  // MVVM_FACTORIES_VIEWMODELCONTROLLERBUILDER_H
