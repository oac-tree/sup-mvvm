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

#ifndef MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_
#define MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_

#include <mvvm/model_export.h>

#include <functional>
#include <memory>
#include <vector>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The ProjectChangedController class tracks changes in all models.
 *
 * It allows to check if one or more models have been changed since the last call of ResetIsChanged.
 * Intended to work together with the Project class. It will take care of calling ResetIsChanged
 * after it is saved. To avoid extra signaling while being in already "changed" mode, the controller
 * reports only once.
 */
class MVVM_MODEL_EXPORT ProjectChangedController
{
public:
  using callback_t = std::function<void()>;
  explicit ProjectChangedController(const std::vector<ISessionModel*>& models,
                                    callback_t project_changed_callback = {});
  ~ProjectChangedController();

  /**
   * @brief Checks if the change in the models has been detected since the last call of
   * ResetIsChanged.
   */
  bool IsChanged() const;

  /**
   * @brief Resets controller to the initial state, pretending that no changes have been detected.
   *
   * The method shall be called by the IProject class after its save.
   */
  void ResetIsChanged();

private:
  struct ProjectChangedControllerImpl;
  std::unique_ptr<ProjectChangedControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_
