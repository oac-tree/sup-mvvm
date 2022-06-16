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

#ifndef MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_
#define MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_

#include <mvvm/model_export.h>

#include <functional>
#include <memory>
#include <vector>

namespace mvvm
{

class ApplicationModel;

//! Tracks changes in all models.
//! Allows to check if one or more models have been changed since last call of ::resetChanged().
//! This is intended to work together with the Project class. It will take care of calling
//! resetChanged after own saving.

//! To avoid extra signaling while being in already "changed" mode, the controller reports only
//! once.

class MVVM_MODEL_EXPORT ProjectChangedController
{
public:
  using callback_t = std::function<void()>;
  explicit ProjectChangedController(const std::vector<ApplicationModel*>& models,
                                    const callback_t& project_changed_callback = {});
  ~ProjectChangedController();

  bool IsChanged() const;

  void ResetIsChanged();

private:
  struct ProjectChangedControllerImpl;
  std::unique_ptr<ProjectChangedControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_CHANGE_CONTROLLER_H_
