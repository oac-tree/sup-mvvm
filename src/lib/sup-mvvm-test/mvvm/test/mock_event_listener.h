/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_TEST_MOCK_EVENT_LISTENER_H_
#define MVVM_TEST_MOCK_EVENT_LISTENER_H_

#include <mvvm/signals/model_event_handler.h>

#include <gmock/gmock.h>

#include <memory>

namespace mvvm::test
{

//! Mock class to validate events coming from the EventHandler.

class MockEventListener
{
public:
  MockEventListener() : m_slot(std::make_unique<mvvm::Slot>()) {}

  MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));

  void SubscribeAll(mvvm::ModelEventHandler* event_handler)
  {
    m_slot = std::make_unique<mvvm::Slot>();

    event_handler->Connect<mvvm::DataChangedEvent>(this, &MockEventListener::OnEvent, m_slot.get());
    event_handler->Connect<mvvm::AboutToInsertItemEvent>(this, &MockEventListener::OnEvent,
                                                         m_slot.get());
    event_handler->Connect<mvvm::ItemInsertedEvent>(this, &MockEventListener::OnEvent,
                                                    m_slot.get());
    event_handler->Connect<mvvm::AboutToRemoveItemEvent>(this, &MockEventListener::OnEvent,
                                                         m_slot.get());
    event_handler->Connect<mvvm::ItemRemovedEvent>(this, &MockEventListener::OnEvent, m_slot.get());

    event_handler->Connect<mvvm::ModelAboutToBeResetEvent>(this, &MockEventListener::OnEvent,
                                                           m_slot.get());
    event_handler->Connect<mvvm::ModelResetEvent>(this, &MockEventListener::OnEvent, m_slot.get());

    event_handler->Connect<mvvm::ModelAboutToBeDestroyedEvent>(this, &MockEventListener::OnEvent,
                                                               m_slot.get());
  }

  void Unsubscribe() { m_slot.reset(); }

  std::unique_ptr<mvvm::Slot> m_slot;
};

}  // namespace mvvm::test

#endif  // MVVM_TEST_MOCK_EVENT_LISTENER_H_
