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

#ifndef MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_
#define MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_

#include <mvvm/providers/abstract_viewmodel_controller.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The ViewModelController class propagates changes from SessionModel to ViewModel.
 */
class ViewModelController : public AbstractViewModelController
{
public:
  explicit ViewModelController(std::unique_ptr<IViewModelController> impl);

  ~ViewModelController() override;

  ViewModelController(const ViewModelController& other) = delete;
  ViewModelController& operator=(const ViewModelController& other) = delete;
  ViewModelController(ViewModelController&& other) = delete;
  ViewModelController& operator=(ViewModelController&&) = delete;

  void OnItemInsertedEvent(const ItemInsertedEvent& event) override;

  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event) override;

  void OnDataChangedEvent(const DataChangedEvent& event) override;

  void OnModelAboutToBeResetEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelResetEvent(const ModelResetEvent& event) override;

  void OnModelAboutToBeDestroyedEvent(const ModelAboutToBeDestroyedEvent& event) override;

  const SessionItem* GetRootItem() const override;

  std::size_t GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  void SetRootItemImpl(SessionItem* root_item) override;

  std::unique_ptr<IViewModelController> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_
