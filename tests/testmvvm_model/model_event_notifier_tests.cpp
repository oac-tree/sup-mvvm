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

#include "mvvm/signals/model_event_notifier.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelEventNotifier class.

class ModelEventNotifierTests : public ::testing::Test
{
public:
  class TestListener
  {
  public:
    void Subscribe(mvvm::ModelEventSubscriberInterface* subscriber)
    {
      m_slot = std::make_unique<mvvm::Slot>();

      auto on_about_to_insert = [this](auto item, auto tagindex)
      { OnAboutToInsertItem(item, tagindex); };
      subscriber->SetOnAboutToInsertItem(on_about_to_insert, m_slot.get());

      auto on_item_inserted = [this](auto item, auto tagindex) { OnItemInserted(item, tagindex); };
      subscriber->SetOnItemInserted(on_item_inserted, m_slot.get());

      auto on_about_to_remove = [this](auto item, auto tagindex)
      { OnAboutToRemoveItem(item, tagindex); };
      subscriber->SetOnAboutToRemoveItem(on_about_to_remove, m_slot.get());

      auto on_item_removed = [this](auto item, auto tagindex) { OnItemRemoved(item, tagindex); };
      subscriber->SetOnItemRemoved(on_item_removed, m_slot.get());

      auto on_data_changed = [this](auto item, auto role) { OnDataChanged(item, role); };
      subscriber->SetOnDataChanged(on_data_changed, m_slot.get());

      auto on_model_about_reset = [this](auto model) { OnModelAboutToBeReset(model); };
      subscriber->SetOnModelAboutToBeReset(on_model_about_reset, m_slot.get());

      auto on_model_reset = [this](auto model) { OnModelReset(model); };
      subscriber->SetOnModelReset(on_model_reset, m_slot.get());

      auto on_model_about_destroyed = [this](auto model) { OnModelAboutToBeDestroyed(model); };
      subscriber->SetOnModelAboutToBeDestroyed(on_model_about_destroyed, m_slot.get());
    }

    void Unsubscribe() { m_slot.reset(); }

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

  ModelEventNotifierTests() { m_listener.Subscribe(&m_notifier); }

  ModelEventNotifier m_notifier;
  TestListener m_listener;
};

//! Checking listener methods when AboutToInsertItem is fired.

TEST_F(ModelEventNotifierTests, AboutToInsertItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(&item, tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.AboutToInsertItemNotify(&item, tag_index);
}

//! Checking listener methods when ItemInserted is fired.

TEST_F(ModelEventNotifierTests, ItemInserted)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(&item, tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.ItemInsertedNotify(&item, tag_index);
}

//! Checking listener methods when AboutToRemoveItem is fired.

TEST_F(ModelEventNotifierTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(&item, tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.AboutToRemoveItemNotify(&item, tag_index);
}

//! Checking listener methods when ItemRemoved is fired.

TEST_F(ModelEventNotifierTests, ItemRemoved)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(&item, tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.ItemRemovedNotify(&item, tag_index);
}

//! Checking listener methods when DataChanged is fired.

TEST_F(ModelEventNotifierTests, DataChanged)
{
  mvvm::SessionItem item;
  int role{42};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.DataChangedNotify(&item, role);
}

TEST_F(ModelEventNotifierTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;
  int role{42};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(&model)).Times(1);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.ModelAboutToBeResetNotify(&model);
}

TEST_F(ModelEventNotifierTests, OnModelReset)
{
  mvvm::SessionModel model;
  int role{42};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(&model)).Times(1);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  m_notifier.ModelResetNotify(&model);
}

TEST_F(ModelEventNotifierTests, OnModelAboutToBeDestroyed)
{
  mvvm::SessionModel model;
  int role{42};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(&model)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(1);

  // triggering action
  m_notifier.ModelAboutToBeDestroyedNotify(&model);
}

// FIXME enable test

// TEST_F(ModelEventNotifierTests, AttemptToEstablishConnectionsTwice)
//{
//   ModelEventNotifier notifier;
//   MockModelListener listener;

//  listener.SubscribeTo(&notifier);

//  EXPECT_THROW(listener.SubscribeTo(&notifier), std::runtime_error);
//}

TEST_F(ModelEventNotifierTests, Unsubscribe)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  TestListener listener;

  listener.Subscribe(&notifier);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  //  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  listener.Unsubscribe();

  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);
}

TEST_F(ModelEventNotifierTests, TwoSubscriptions)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  TestListener listener1;
  TestListener listener2;

  listener1.Subscribe(&notifier);
  listener2.Subscribe(&notifier);

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(1);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);

  // triggering action
  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);
}

TEST_F(ModelEventNotifierTests, UnsubscribeOne)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  TestListener listener1;
  TestListener listener2;

  listener1.Subscribe(&notifier);
  listener2.Subscribe(&notifier);

  listener1.Unsubscribe();

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(0);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);

  // triggering action
  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(0);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(0);

  listener2.Unsubscribe();

  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);
}
