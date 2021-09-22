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

#include <gtest/gtest.h>

using namespace ModelView;
using ::testing::_;

//! Tests for ModelEventNotifier class.

class ModelEventNotifierTest : public ::testing::Test
{
public:
  ModelEventNotifierTest() { m_notifier.EstablishConnections(&m_listener); }

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

  // triggering action
  m_notifier.DataChangedNotify(&item, role);
}
