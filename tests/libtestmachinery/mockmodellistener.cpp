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

#include "mockmodellistener.h"

MockModelListener::~MockModelListener()
{
  if (m_notifier)
  {
    UnsubscribeFrom(m_notifier);
  }
}

void MockModelListener::SetNotifier(mvvm::ModelEventSubscriberInterface *notifier)
{
  m_notifier = notifier;
}

void MockModelListener::SubscribeTo(mvvm::ModelEventSubscriberInterface *notifier)
{
  m_notifier = notifier;
  m_notifier->Subscribe(this);
}

void MockModelListener::UnsubscribeFrom(mvvm::ModelEventSubscriberInterface *notifier)
{
  notifier->Unsubscribe(this);
}
