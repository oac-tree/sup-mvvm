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
#include <mvvm/signals/model_event_handler.h>

//! Mocking class to test ModelEventNotifier when it is called from ModelComposer.

class MockModelEventNotifier : public mvvm::ModelEventHandler
{
public:

  // Using older MOCK_METHOD1(2) because of problems on CODAC and MacOs

//  MOCK_METHOD(void, AboutToInsertItemNotify,
//              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD2(AboutToInsertItemNotify,
              void(mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

//  MOCK_METHOD(void, ItemInsertedNotify,
//              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));
  MOCK_METHOD2(ItemInsertedNotify,
              void(mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

//  MOCK_METHOD(void, AboutToRemoveItemNotify,
//              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));
  MOCK_METHOD2(AboutToRemoveItemNotify,
              void(mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

//  MOCK_METHOD(void, ItemRemovedNotify,
//              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));
  MOCK_METHOD2(ItemRemovedNotify,
              void(mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

//  MOCK_METHOD(void, DataChangedNotify, (mvvm::SessionItem * item, int role), (override));
  MOCK_METHOD2(DataChangedNotify, void(mvvm::SessionItem * item, int role));

//  MOCK_METHOD(void, ModelAboutToBeResetNotify, (mvvm::SessionModelInterface * model), (override));
  MOCK_METHOD1(ModelAboutToBeResetNotify, void(mvvm::SessionModelInterface * model));

//  MOCK_METHOD(void, ModelResetNotify, (mvvm::SessionModelInterface * model), (override));
  MOCK_METHOD1(ModelResetNotify, void(mvvm::SessionModelInterface * model));

//  MOCK_METHOD(void, ModelAboutToBeDestroyedNotify, (mvvm::SessionModelInterface * model),
//              (override));
  MOCK_METHOD1(ModelAboutToBeDestroyedNotify, void(mvvm::SessionModelInterface * model));
};

//! A decorator to wrap MockModelNotifier for later use with unique_ptr (gmock doesn't like
//! to put mocking objects into unique_ptr).
class MockModelEventNotifierDecorator : public mvvm::ModelEventHandler
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
