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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
#define MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_

#include <mvvm/providers/abstract_viewmodel_controller.h>

namespace mvvm
{

class IConnectableShapeFactory;

/**
 * @brief The ConnectableViewModelController syncronizes a model with node items and objects on the
 * graphics scene.
 */
class ConnectableViewModelController : public AbstractViewModelController
{
public:
  explicit ConnectableViewModelController(std::unique_ptr<IConnectableShapeFactory> shape_factory);

  ~ConnectableViewModelController() override;

  ConnectableViewModelController(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController& operator=(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController(ConnectableViewModelController&& other) = delete;
  ConnectableViewModelController& operator=(ConnectableViewModelController&&) = delete;

private:
  void SetRootItemImpl(SessionItem* root_item) override;
  void SubscribeImpl(ISessionModel* model) override;

  std::unique_ptr<IConnectableShapeFactory> m_shape_factory;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
