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

#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/signals/signalslot.h"

#include <gmock/gmock.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
class ApplicationModel;
}  // namespace mvvm

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockModelListener
{
public:
  MockModelListener(mvvm::ApplicationModel* model = nullptr);

  void Subscribe(mvvm::ModelEventSubscriberInterface* subscriber);
  void Unsubscribe();

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
  std::unique_ptr<mvvm::Slot> m_slot;
};

#endif  // MOCKMODELLISTENER_H
