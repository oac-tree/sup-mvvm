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

#ifndef MVVM_EXPERIMENTAL_EVENT_SUBSCRIBER_INTERFACE_H_
#define MVVM_EXPERIMENTAL_EVENT_SUBSCRIBER_INTERFACE_H_

#include <mvvm/experimental/event_types.h>

#include <functional>

namespace mvvm::experimental
{

class ModelEventSubscriberInterface
{
public:

  void Subscribe(int type_id, std::function<void(const event_t& event)>);

  template<typename T> void Subscribe()
  {

  }
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_EVENT_SUBSCRIBER_INTERFACE_H_
