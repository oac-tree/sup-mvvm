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

#include "mvvm/model/application_model.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/test/mock_model_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests of ApplicationModel in simple undo/redo scenario.
 */
class ApplicationModelSimpleUndoTests : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListener>;

  ApplicationModelSimpleUndoTests() = default;

  ApplicationModel m_model;
};

TEST_F(ApplicationModelSimpleUndoTests, SetData)
{
  const role_t role = DataRole::kData;

  auto item = m_model.InsertItem<SessionItem>();
  EXPECT_FALSE(utils::IsValid(item->Data(role)));

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  const DataChangedEvent data_changed_event{item, role};
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);

  const variant_t expected_data(42);
  EXPECT_TRUE(m_model.SetData(item, 42, DataRole::kData));

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(commands->GetCommandCount(), 1);

  // undoing the command
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);
  commands->Undo();

  // checking the data and status of command stack
  EXPECT_FALSE(utils::IsValid(item->Data(role)));
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 0);
  EXPECT_EQ(commands->GetCommandCount(), 1);

  // redoing command
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);
  commands->Redo();

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(commands->GetCommandCount(), 1);
}

TEST_F(ApplicationModelSimpleUndoTests, SetSameData)
{
  const role_t role = DataRole::kData;
  variant_t expected_data(42);

  // creating item and setting initial data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(expected_data);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  // setting same data
  EXPECT_FALSE(m_model.SetData(item, 42, DataRole::kData));

  // no data change
  EXPECT_EQ(item->Data(), variant_t(42));

  // no commands should be in the stack
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 0);
  EXPECT_EQ(commands->GetCommandCount(), 0);
}

TEST_F(ApplicationModelSimpleUndoTests, SetDataSeveralTimes)
{
  const role_t role = DataRole::kData;

  // creating item and setting initial data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(10);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  const DataChangedEvent data_changed_event{item, role};
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(3);

  // setting data several times
  EXPECT_TRUE(m_model.SetData(item, 11, DataRole::kData));
  EXPECT_TRUE(m_model.SetData(item, 12, DataRole::kData));
  EXPECT_TRUE(m_model.SetData(item, 13, DataRole::kData));

  // checking data and stack
  EXPECT_EQ(item->Data(), variant_t(13));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(commands->GetCommandCount(), 3);

  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(2);

  // undoing twice and checking status
  commands->Undo();
  commands->Undo();
  EXPECT_EQ(item->Data(), variant_t(11));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(commands->GetCommandCount(), 3);

  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);

  // while being in partly undone state, make a new command
  // this will remove two last commands
  EXPECT_TRUE(m_model.SetData(item, 14, DataRole::kData));

  EXPECT_EQ(item->Data(), variant_t(14));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetCommandCount(), 2);

  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(2);

  // two undo should give us initial value
  commands->Undo();
  commands->Undo();

  EXPECT_EQ(item->Data(), variant_t(10));
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 0);
  EXPECT_EQ(commands->GetCommandCount(), 2);

  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);

  commands->Redo();
  EXPECT_EQ(item->Data(), variant_t(11));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 1);

  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);

  // back on top of the stack
  commands->Redo();
  EXPECT_EQ(item->Data(), variant_t(14));
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
}

TEST_F(ApplicationModelSimpleUndoTests, RemoveData)
{
  const role_t role = DataRole::kData;

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  const DataChangedEvent data_changed_event{item, role};
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);

  // invalid variant removes the data
  EXPECT_TRUE(m_model.SetData(item, variant_t{}, role));

  EXPECT_FALSE(item->HasData(role));
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(commands->GetCommandCount(), 1);

  // undoing the command restores the data
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);
  commands->Undo();

  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_EQ(commands->GetIndex(), 0);

  // redoing the command removes the data again
  EXPECT_CALL(listener, OnDataChanged(data_changed_event)).Times(1);
  commands->Redo();

  EXPECT_FALSE(item->HasData(role));
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(commands->GetCommandCount(), 1);
}

TEST_F(ApplicationModelSimpleUndoTests, RemoveDataWhenNoData)
{
  auto item = m_model.InsertItem<SessionItem>();

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  // removing absent data does nothing
  EXPECT_FALSE(m_model.SetData(item, variant_t{}, DataRole::kData));

  EXPECT_FALSE(item->HasData(DataRole::kData));
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_EQ(commands->GetCommandCount(), 0);
}

TEST_F(ApplicationModelSimpleUndoTests, RemoveDataThenSetAnotherType)
{
  const role_t role = DataRole::kData;

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();

  // after removal the data can change its type
  EXPECT_TRUE(m_model.SetData(item, variant_t{}, role));
  EXPECT_TRUE(m_model.SetData(item, std::string("abc"), role));
  EXPECT_EQ(item->Data(), variant_t(std::string("abc")));
  EXPECT_EQ(commands->GetCommandCount(), 2);

  commands->Undo();
  EXPECT_FALSE(item->HasData(role));

  commands->Undo();
  EXPECT_EQ(item->Data(), variant_t(42));
}

TEST_F(ApplicationModelSimpleUndoTests, SetDataOfAnotherType)
{
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();

  // data of another type is refused while the data is present
  EXPECT_THROW(m_model.SetData(item, std::string("abc"), DataRole::kData),
               InvalidOperationException);
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_EQ(commands->GetCommandCount(), 0);

  // the model keeps working
  EXPECT_TRUE(m_model.SetData(item, 43, DataRole::kData));
  commands->Undo();
  EXPECT_EQ(item->Data(), variant_t(42));
}

TEST_F(ApplicationModelSimpleUndoTests, SetDataThenRemove)
{
  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  m_model.SetUndoEnabled(true, 100);
  auto commands = m_model.GetCommandStack();
  mock_listener_t listener(&m_model);

  const AboutToInsertItemEvent event1{parent, TagIndex{"tag1", 0}};
  EXPECT_CALL(listener, OnAboutToInsertItem(event1)).Times(1);
  const ItemInsertedEvent event2{parent, TagIndex{"tag1", 0}};
  EXPECT_CALL(listener, OnItemInserted(event2)).Times(1);

  auto child1 = m_model.InsertItem<SessionItem>(parent);

  const DataChangedEvent event3{child1, DataRole::kData};

  EXPECT_CALL(listener, OnDataChanged(event3)).Times(1);
  child1->SetData(42);

  EXPECT_CALL(listener, OnDataChanged(event3)).Times(1);
  EXPECT_TRUE(m_model.SetData(child1, 43, DataRole::kData));

  const AboutToRemoveItemEvent event4{m_model.GetRootItem(), TagIndex::First()};
  EXPECT_CALL(listener, OnAboutToRemoveItem(event4)).Times(1);

  const ItemRemovedEvent event5{m_model.GetRootItem(), TagIndex::First()};
  EXPECT_CALL(listener, OnItemRemoved(event5)).Times(1);

  auto taken = m_model.TakeItem(m_model.GetRootItem(), TagIndex::First());

  // status of stack
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 4);
  EXPECT_EQ(commands->GetCommandCount(), 4);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undo command

  const AboutToInsertItemEvent event6{m_model.GetRootItem(), TagIndex::First()};
  EXPECT_CALL(listener, OnAboutToInsertItem(event6)).Times(1);

  const ItemInsertedEvent event7{m_model.GetRootItem(), TagIndex::First()};
  EXPECT_CALL(listener, OnItemInserted(event7)).Times(1);

  commands->Undo();

  auto restored_parent = utils::ChildAt(m_model.GetRootItem(), 0);
  auto restored_child = utils::ChildAt(restored_parent, 0);
  EXPECT_EQ(restored_child->Data<int>(), 43);

  const DataChangedEvent event8{restored_child, DataRole::kData};
  EXPECT_CALL(listener, OnDataChanged(event8)).Times(1);
  commands->Undo();

  // status of stack
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetCommandCount(), 4);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(restored_child->Data<int>(), 42);
}

}  // namespace mvvm::test
