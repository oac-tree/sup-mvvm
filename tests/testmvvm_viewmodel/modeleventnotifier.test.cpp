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

#include "mvvm/viewmodel/modeleventnotifier.h"

#include "mockmodellistener.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace ModelView;
using ::testing::_;

//! Tests for ModelEventNotifier class.

class ModelEventNotifierTest : public ::testing::Test
{
public:
  ModelEventNotifierTest() { m_notifier.Subscribe(&m_listener); }

  ModelEventNotifier m_notifier;
  MockModelListener m_listener;
};

//! Checking listener methods when AboutToInsertItem is fired.

TEST_F(ModelEventNotifierTest, AboutToInsertItem)
{
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};

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

TEST_F(ModelEventNotifierTest, ItemInserted)
{
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};

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

TEST_F(ModelEventNotifierTest, AboutToRemoveItem)
{
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};

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

TEST_F(ModelEventNotifierTest, ItemRemoved)
{
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};

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

TEST_F(ModelEventNotifierTest, DataChanged)
{
  ModelView::SessionItem item;
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

TEST_F(ModelEventNotifierTest, OnModelAboutToBeReset)
{
  ModelView::SessionModel model;
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

TEST_F(ModelEventNotifierTest, OnModelReset)
{
  ModelView::SessionModel model;
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

TEST_F(ModelEventNotifierTest, OnModelAboutToBeDestroyed)
{
  ModelView::SessionModel model;
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

TEST_F(ModelEventNotifierTest, AttemptToEstablishConnectionsTwice)
{
  ModelEventNotifier notifier;
  MockModelListener listener;

  notifier.Subscribe(&listener);

  EXPECT_THROW(notifier.Subscribe(&listener), std::runtime_error);
}

TEST_F(ModelEventNotifierTest, Unsubscribe)
{
  ModelView::SessionModel model;
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  MockModelListener listener;

  notifier.Subscribe(&listener);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  notifier.Unsubscribe(&listener);

  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);
}

TEST_F(ModelEventNotifierTest, TwoSubscriptions)
{
  ModelView::SessionModel model;
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  MockModelListener listener1;
  MockModelListener listener2;

  notifier.Subscribe(&listener1);
  notifier.Subscribe(&listener2);

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

TEST_F(ModelEventNotifierTest, UnsubscribeOne)
{
  ModelView::SessionModel model;
  ModelView::SessionItem item;
  ModelView::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventNotifier notifier;
  MockModelListener listener1;
  MockModelListener listener2;

  notifier.Subscribe(&listener1);
  notifier.Subscribe(&listener2);

  notifier.Unsubscribe(&listener1);

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

  notifier.Unsubscribe(&listener2);

  notifier.AboutToInsertItemNotify(&item, tag_index);
  notifier.ItemInsertedNotify(&item, tag_index);
  notifier.AboutToRemoveItemNotify(&item, tag_index);
  notifier.ItemRemovedNotify(&item, tag_index);
  notifier.DataChangedNotify(&item, role);
  notifier.ModelAboutToBeResetNotify(&model);
  notifier.ModelResetNotify(&model);
  notifier.ModelAboutToBeDestroyedNotify(&model);
}
