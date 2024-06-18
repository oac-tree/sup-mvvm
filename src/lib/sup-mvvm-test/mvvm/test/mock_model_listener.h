/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_LISTENER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_LISTENER_H_

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/signals/model_listener.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

//! Mocking class to test events coming from the model.

class MockModelListener : public mvvm::ModelListener<mvvm::ISessionModel>
{
public:
  explicit MockModelListener(mvvm::ISessionModel* model);

  MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));
};

//! Mocking class to test events coming from the model. The difference with the class above is a
//! connection to separate slots to simplify wiring of unit tests.

class MockModelListenerV2 : public mvvm::ModelListener<mvvm::ISessionModel>
{
public:
  explicit MockModelListenerV2(const mvvm::ISessionModel* model);

  MOCK_METHOD(void, OnAboutToInsertItem, (const mvvm::AboutToInsertItemEvent& event), ());

  MOCK_METHOD(void, OnItemInserted, (const mvvm::ItemInsertedEvent& event), ());

  MOCK_METHOD(void, OnAboutToRemoveItem, (const mvvm::AboutToRemoveItemEvent& event), ());

  MOCK_METHOD(void, OnItemRemoved, (const mvvm::ItemRemovedEvent& event), ());

  MOCK_METHOD(void, OnDataChanged, (const mvvm::DataChangedEvent& event), ());

  MOCK_METHOD(void, OnModelAboutToBeReset, (const mvvm::ModelAboutToBeResetEvent& event), ());

  MOCK_METHOD(void, OnModelReset, (const mvvm::ModelResetEvent& event), ());

  MOCK_METHOD(void, OnModelAboutToBeDestroyed, (const mvvm::ModelAboutToBeDestroyedEvent& event),
              ());

  // we wrap mocking methods into other methods to be able to do additional testing in addition to
  // mocking

  void OnAboutToInsertItemEvent(const mvvm::AboutToInsertItemEvent& event)
  {
    OnAboutToInsertItem(event);
  }

  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event) { OnItemInserted(event); }

  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event)
  {
    OnAboutToRemoveItem(event);
  }

  void OnItemRemovedEvent(const mvvm::ItemRemovedEvent& event) { OnItemRemoved(event); }

  void OnDataChangedEvent(const mvvm::DataChangedEvent& event) { OnDataChanged(event); }

  void OnModelAboutToBeResetEvent(const mvvm::ModelAboutToBeResetEvent& event)
  {
    OnModelAboutToBeReset(event);
  }

  void OnModelResetEvent(const mvvm::ModelResetEvent& event) { OnModelReset(event); }

  void OnModelAboutToBeDestroyedEvent(const mvvm::ModelAboutToBeDestroyedEvent& event)
  {
    OnModelAboutToBeDestroyed(event);
  }
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_LISTENER_H_
