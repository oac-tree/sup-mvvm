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

#include "mvvm/signals/itemconnectutils.h"

#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/model/applicationmodel.h"

namespace
{
mvvm::ModelEventSubscriberInterface *GetSubscriber(const mvvm::SessionItem *item)
{
  if (!item->GetModel())
  {
    throw std::runtime_error("Item doesn't have a model");
  }

  if (auto model = dynamic_cast<mvvm::ApplicationModel *>(item->GetModel()); model)
  {
    return model->GetSubscriber();
  }
  throw std::runtime_error("Item's model doesn't have signaling capabilities");
}
}  // namespace

namespace mvvm::connect
{

Connection OnDataChange(SessionItem *source, const Callbacks::item_int_t &f, Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // callback will filtering capabilities to call user callback only when the event had happened
  // with our source
  auto on_data_change = [&f, source](SessionItem *item, int role)
  {
    if (item == source)
    {
      f(item, role);  // calling user provided callback
    }
  };

  return subscriber->SetOnDataChanged(on_data_change, slot);
}

}  // namespace mvvm::connect
