/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROJECT_MODEL_HAS_CHANGED_CONTROLLER_H_
#define MVVM_PROJECT_MODEL_HAS_CHANGED_CONTROLLER_H_

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/signals/model_listener.h>

#include <functional>

namespace mvvm
{

//! Tracks changes in the model.
//! Allows to check if model has been changed (e.g. modified, inserted or removed items) since last
//! call of resetChanged().

class MVVM_MODEL_EXPORT ModelHasChangedController : public ModelListener<SessionModelInterface>
{
public:
  using callback_t = std::function<void()>;
  explicit ModelHasChangedController(SessionModelInterface* model, const callback_t& callback = {});

  bool IsChanged() const;

  void ResetIsChanged();

private:
  void OnChange();
  bool m_has_changed{false};
  callback_t m_callback;  //! informs the user about change in the model
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_MODEL_HAS_CHANGED_CONTROLLER_H_
