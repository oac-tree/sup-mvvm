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

#ifndef MOCKMODELLISTENER_H
#define MOCKMODELLISTENER_H

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/interfaces/modeleventnotifierinterface.h"
#include "mvvm/interfaces/modeleventsubscriberinterface.h"

#include <gmock/gmock.h>

namespace mvvm
{
class SessionItem;
class TagIndex;
class SessionModel;
}  // namespace mvvm

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockModelListener : public mvvm::ModelEventListenerInterface
{
public:
  ~MockModelListener();

  void SetNotifier(mvvm::ModelEventSubscriberInterface* notifier);

  void SubscribeTo(mvvm::ModelEventSubscriberInterface* notifier);
  void UnsubscribeFrom(mvvm::ModelEventSubscriberInterface* notifier);

  MOCK_METHOD2(OnAboutToInsertItem,
               void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnItemInserted, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnAboutToRemoveItem,
               void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnItemRemoved, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnDataChanged, void(mvvm::SessionItem* item, int role));

  MOCK_METHOD1(OnModelAboutToBeReset, void(mvvm::SessionModel* model));

  MOCK_METHOD1(OnModelReset, void(mvvm::SessionModel* model));

  MOCK_METHOD1(OnModelAboutToBeDestroyed, void(mvvm::SessionModel* model));

protected:
  mvvm::ModelEventSubscriberInterface* m_notifier;
};

#endif  // MOCKMODELLISTENER_H
