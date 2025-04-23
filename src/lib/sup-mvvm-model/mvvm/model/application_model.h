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

#ifndef MVVM_MODEL_APPLICATION_MODEL_H_
#define MVVM_MODEL_APPLICATION_MODEL_H_

#include <mvvm/model/session_model.h>

#include <memory>

namespace mvvm
{

class ModelEventHandler;
class ICommandStack;

/**
 * @brief The ApplicationModel class is the main model to hold application data.
 *
 * It extends SessionModel with signalling capabilites, and undo/redo.
 */
class MVVM_MODEL_EXPORT ApplicationModel : public SessionModel
{
public:
  using ISessionModel::InsertItem;

  explicit ApplicationModel(std::string model_type = {});
  ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool);

  ~ApplicationModel() override;

  ModelEventHandler* GetEventHandler() const override;

  ICommandStack* GetCommandStack() const override;

  void CheckIn(SessionItem* item) override;

  /**
   * @brief Enables or disables undo/redo framework.
   *
   * When disabled, all command history is cleared.
   *
   * @param value To enable undo.
   * @param undo_limit Maximum number of commands stored in undo stack (0 = no limits).
   */
  void SetUndoEnabled(bool value, std::size_t undo_limit = 0);

private:
  struct ApplicationModelImpl;
  std::unique_ptr<ApplicationModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_H_
