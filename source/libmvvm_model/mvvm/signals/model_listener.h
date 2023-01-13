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

#ifndef MVVM_SIGNALS_MODEL_LISTENER_H_
#define MVVM_SIGNALS_MODEL_LISTENER_H_

#include <mvvm/signals/model_listener_base.h>

namespace mvvm
{

//! Templated class for all objects willing to listen for changes in concrete SessionModel.
//! It can be used as a base class, or as a standalone listener. In the last case other objects
//! can connect to the model via the listener with their callbacks. The callbacks will be called
//! as long as the listener is alive.

template <typename T>
class ModelListener : public ModelListenerBase
{
public:
  explicit ModelListener(T* model) : ModelListenerBase(model) {}
  T* GetModel() const { return static_cast<T*>(GetModelBase()); }
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_H_
