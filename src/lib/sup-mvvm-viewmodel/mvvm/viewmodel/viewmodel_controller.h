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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_

#include <mvvm/viewmodel/abstract_viewmodel_controller.h>

#include <memory>

namespace mvvm
{
class SessionModelInterface;
class ViewModelBase;
class ChildrenStrategyInterface;
class RowStrategyInterface;
class ViewModelControllerImpl;

//! Propagates changes from SessionModel to its ViewModelBase.
//!
class MVVM_VIEWMODEL_EXPORT ViewModelController : public AbstractViewModelController
{
public:
  explicit ViewModelController(ViewModelBase* view_model);
  ViewModelController(SessionModelInterface* model, ViewModelBase* view_model);
  ~ViewModelController() override;

  void SetChildrenStrategy(std::unique_ptr<ChildrenStrategyInterface> children_strategy);

  void SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy);

  void OnModelEvent(const ItemInsertedEvent& event) override;

  void OnModelEvent(const AboutToRemoveItemEvent& event) override;

  void OnModelEvent(const DataChangedEvent& event) override;

  void OnModelEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelEvent(const ModelResetEvent& event) override;

  void OnModelEvent(const ModelAboutToBeDestroyedEvent& event) override;

  const SessionItem* GetRootItem() const override;

  void SetItem(SessionItem* root_item) override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::unique_ptr<ViewModelControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_
