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

#ifndef MVVM_SIGNALS_MODELLISTENER_H
#define MVVM_SIGNALS_MODELLISTENER_H

#include <mvvm/signals/model_listener_base.h>

namespace mvvm
{

//! Templated class for all objects willing to listen for changes in concrete SessionModel.

template <typename T>
class ModelListener : public ModelListenerBase
{
public:
  explicit ModelListener(T* model) : ModelListenerBase(model) {}
  T* GetModel() const { return static_cast<T*>(GetCurrentModel()); }
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODELLISTENER_H
