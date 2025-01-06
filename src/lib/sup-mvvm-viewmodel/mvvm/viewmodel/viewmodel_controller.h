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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_

#include <mvvm/viewmodel/abstract_viewmodel_controller.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The ViewModelController class propagates changes from SessionModel to ViewModel.
 */
class MVVM_VIEWMODEL_EXPORT ViewModelController : public AbstractViewModelController
{
public:
  explicit ViewModelController(std::unique_ptr<IViewModelController> impl);

  ~ViewModelController() override;

  void OnModelEvent(const ItemInsertedEvent& event) override;

  void OnModelEvent(const AboutToRemoveItemEvent& event) override;

  void OnModelEvent(const DataChangedEvent& event) override;

  void OnModelEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelEvent(const ModelResetEvent& event) override;

  void OnModelEvent(const ModelAboutToBeDestroyedEvent& event) override;

  const SessionItem* GetRootItem() const override;

  int GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  void SetRootItemImpl(SessionItem* root_item) override;

  std::unique_ptr<IViewModelController> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_
