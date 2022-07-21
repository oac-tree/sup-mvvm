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

#ifndef TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_NOTIFIER_H_
#define TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_NOTIFIER_H_

#include <gmock/gmock.h>
#include <mvvm/interfaces/model_event_notifier_interface.h>

//! Mocking class to test ModelEventNotifierInterface when it is called from ModelComposer.

class MockModelEventNotifier : public mvvm::ModelEventNotifierInterface
{
public:
  MOCK_METHOD(void, AboutToInsertItemNotify,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD(void, ItemInsertedNotify,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD(void, AboutToRemoveItemNotify,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD(void, ItemRemovedNotify,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD(void, DataChangedNotify, (mvvm::SessionItem * item, int role), (override));

  MOCK_METHOD(void, ModelAboutToBeResetNotify, (mvvm::SessionModelInterface * model), (override));

  MOCK_METHOD(void, ModelResetNotify, (mvvm::SessionModelInterface * model), (override));

  MOCK_METHOD(void, ModelAboutToBeDestroyedNotify, (mvvm::SessionModelInterface * model),
              (override));
};

//! A decorator to wrap MockModelNotifier for later use with unique_ptr (gmock doesn't like
//! to put mocking objects into unique_ptr).
class MockModelEventNotifierDecorator : public mvvm::ModelEventNotifierInterface
{
public:
  explicit MockModelEventNotifierDecorator(MockModelEventNotifier* notifier)
      : m_mock_notifier(notifier)
  {
  }

  void AboutToInsertItemNotify(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index) override
  {
    m_mock_notifier->AboutToInsertItemNotify(parent, tag_index);
  }

  void ItemInsertedNotify(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index) override
  {
    m_mock_notifier->ItemInsertedNotify(parent, tag_index);
  }

  void AboutToRemoveItemNotify(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index) override
  {
    m_mock_notifier->AboutToRemoveItemNotify(parent, tag_index);
  }

  void ItemRemovedNotify(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index) override
  {
    m_mock_notifier->ItemRemovedNotify(parent, tag_index);
  }

  void DataChangedNotify(mvvm::SessionItem* item, int role) override
  {
    m_mock_notifier->DataChangedNotify(item, role);
  }

  void ModelAboutToBeResetNotify(mvvm::SessionModelInterface* model) override
  {
    m_mock_notifier->ModelAboutToBeResetNotify(model);
  }

  void ModelResetNotify(mvvm::SessionModelInterface* model) override
  {
    m_mock_notifier->ModelResetNotify(model);
  }

  void ModelAboutToBeDestroyedNotify(mvvm::SessionModelInterface* model) override
  {
    m_mock_notifier->ModelAboutToBeDestroyedNotify(model);
  }

  MockModelEventNotifier* m_mock_notifier{nullptr};
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_EVENT_NOTIFIER_H_
