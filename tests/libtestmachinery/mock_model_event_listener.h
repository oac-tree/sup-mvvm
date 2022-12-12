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

#ifndef TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_LISTENER_H_
#define TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_LISTENER_H_

#include <gmock/gmock.h>
#include <mvvm/signals/model_event_handler.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

class MockModelEventListener
{
public:
  MockModelEventListener() : m_slot(std::make_unique<mvvm::Slot>()) {}

  MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));

  void SubscribeAll(mvvm::ModelEventHandler* event_handler)
  {
    m_slot = std::make_unique<mvvm::Slot>();

    event_handler->Connect<mvvm::DataChangedEvent>(this, &MockModelEventListener::OnEvent,
                                                   m_slot.get());
    event_handler->Connect<mvvm::AboutToInsertItemEvent>(this, &MockModelEventListener::OnEvent,
                                                         m_slot.get());
    event_handler->Connect<mvvm::ItemInsertedEvent>(this, &MockModelEventListener::OnEvent,
                                                    m_slot.get());
    event_handler->Connect<mvvm::AboutToRemoveItemEvent>(this, &MockModelEventListener::OnEvent,
                                                         m_slot.get());
    event_handler->Connect<mvvm::ItemRemovedEvent>(this, &MockModelEventListener::OnEvent,
                                                   m_slot.get());

    event_handler->Connect<mvvm::ModelAboutToBeResetEvent>(this, &MockModelEventListener::OnEvent,
                                                           m_slot.get());
    event_handler->Connect<mvvm::ModelResetEvent>(this, &MockModelEventListener::OnEvent,
                                                  m_slot.get());

    event_handler->Connect<mvvm::ModelAboutToBeDestroyedEvent>(
        this, &MockModelEventListener::OnEvent, m_slot.get());
  }

  void Unsubscribe() { m_slot.reset(); }

  std::unique_ptr<mvvm::Slot> m_slot;
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_LISTENER_H_
