/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H
#define MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"

namespace mvvm
{
class SessionItem;
class TagIndex;
class ModelEventSubscriberInterface;
class SessionModel;

//! Interface for all objects that need to listen for events happening with the SessionModel.

class MVVM_MODEL_EXPORT ModelEventListenerInterface
{
public:
  virtual ~ModelEventListenerInterface() = default;

  //! Subscribe to a given notifier
  virtual void SubscribeTo(ModelEventSubscriberInterface* notifier) = 0;

  //! Unsubscribe from a given notifier
  virtual void UnsubscribeFrom(ModelEventSubscriberInterface* notifier) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H
