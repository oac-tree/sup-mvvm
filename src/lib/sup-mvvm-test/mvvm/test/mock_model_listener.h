/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_TEST_MOCK_MODEL_LISTENER_H_
#define MVVM_TEST_MOCK_MODEL_LISTENER_H_

#include <mvvm/signals/model_listener.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockModelListener is a mocking class to test events coming from the model.
 */
class MockModelListener : public ModelListener
{
public:
  explicit MockModelListener(const ISessionModel* model);

  MOCK_METHOD(void, OnAboutToInsertItem, (const AboutToInsertItemEvent& event), ());

  MOCK_METHOD(void, OnItemInserted, (const ItemInsertedEvent& event), ());

  MOCK_METHOD(void, OnAboutToRemoveItem, (const AboutToRemoveItemEvent& event), ());

  MOCK_METHOD(void, OnItemRemoved, (const ItemRemovedEvent& event), ());

  MOCK_METHOD(void, OnItemDestroyed, (const ItemDestroyedEvent& event), ());

  MOCK_METHOD(void, OnDataChanged, (const DataChangedEvent& event), ());

  MOCK_METHOD(void, OnModelAboutToBeReset, (const ModelAboutToBeResetEvent& event), ());

  MOCK_METHOD(void, OnModelReset, (const ModelResetEvent& event), ());

  MOCK_METHOD(void, OnModelAboutToBeDestroyed, (const ModelAboutToBeDestroyedEvent& event), ());

  // we wrap mocking methods into other methods to be able to do additional testing in addition to
  // mocking

  void OnAboutToInsertItemEvent(const AboutToInsertItemEvent& event) { OnAboutToInsertItem(event); }

  void OnItemInsertedEvent(const ItemInsertedEvent& event) { OnItemInserted(event); }

  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event) { OnAboutToRemoveItem(event); }

  void OnItemRemovedEvent(const ItemRemovedEvent& event) { OnItemRemoved(event); }

  void OnItemDestroyedEvent(const ItemDestroyedEvent& event) { OnItemDestroyed(event); }

  void OnDataChangedEvent(const DataChangedEvent& event) { OnDataChanged(event); }

  void OnModelAboutToBeResetEvent(const ModelAboutToBeResetEvent& event)
  {
    OnModelAboutToBeReset(event);
  }

  void OnModelResetEvent(const ModelResetEvent& event) { OnModelReset(event); }

  void OnModelAboutToBeDestroyedEvent(const ModelAboutToBeDestroyedEvent& event)
  {
    OnModelAboutToBeDestroyed(event);
  }
};

}  // namespace mvvm::test

#endif  // MVVM_TEST_MOCK_MODEL_LISTENER_H_
