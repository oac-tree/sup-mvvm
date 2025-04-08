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

#include "mvvm/model/application_model.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/test/mock_model_listener.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ApplicationModel in undo/redo scenario.

class ApplicationModelUndoTests : public ::testing::Test
{
public:
  ApplicationModelUndoTests() : m_model("TestModelType", m_pool) {}

  ApplicationModel m_model;
  static std::shared_ptr<ItemPool> m_pool;
};

std::shared_ptr<ItemPool> ApplicationModelUndoTests::m_pool = std::make_shared<ItemPool>();

//! Inserting item, changing the data, removing item, undoing.
TEST_F(ApplicationModelUndoTests, InsertItemSetDataRemoveItem)
{
  m_model.SetUndoEnabled(true);

  auto commands = m_model.GetCommandStack();

  EXPECT_EQ(commands->GetCommandCount(), 0);

  // inserting item and setting the data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  // status of command stack with two commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetCommandCount(), 2);

  // removing item
  m_model.RemoveItem(item);

  // status of command stack with three commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(commands->GetCommandCount(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undoing last removal
  commands->Undo();

  // status of command stack
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetCommandCount(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(m_model.GetRootItem()->GetItem(TagIndex())->Data(), variant_t(42));
}

//! Add GraphItem and Data1DItem, add data to graph, undo, then redo. GraphItem should be pointing
//! again to Data1DItem. This is real bug case.
TEST_F(ApplicationModelUndoTests, InsertDataAndGraph)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  auto data_item = m_model.InsertItem<Data1DItem>();
  auto graph_item = m_model.InsertItem<GraphItem>();
  graph_item->SetDataItem(data_item);

  auto data_item_identifier = data_item->GetIdentifier();
  auto graph_item_identifier = graph_item->GetIdentifier();

  // model has two elements, graph is pointing to the data
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(commands->GetCommandCount(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(graph_item->GetDataItem(), data_item);

  // checking pool
  EXPECT_EQ(m_pool->ItemForKey(data_item_identifier), data_item);
  EXPECT_EQ(m_pool->ItemForKey(graph_item_identifier), graph_item);

  // undoing once (setDataItem operation)
  commands->Undo();

  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetCommandCount(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(graph_item->GetDataItem(), nullptr);

  // undoing two more times item
  commands->Undo();
  commands->Undo();
  EXPECT_EQ(commands->GetIndex(), 0);
  EXPECT_EQ(commands->GetCommandCount(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // redoing (dataItem is back)
  commands->Redo();
  EXPECT_EQ(commands->GetIndex(), 1);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto restored_data_item = utils::GetTopItem<Data1DItem>(&m_model);
  EXPECT_EQ(restored_data_item->GetIdentifier(), data_item_identifier);
  EXPECT_EQ(m_pool->ItemForKey(data_item_identifier), restored_data_item);

  // redoing (GraphItem) is back
  commands->Redo();
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  auto restored_graph_item = utils::GetTopItem<GraphItem>(&m_model);
  EXPECT_EQ(restored_graph_item->GetIdentifier(), graph_item_identifier);
  EXPECT_EQ(restored_graph_item->GetDataItem(), nullptr);

  // redoing (graph is linked with data again)
  commands->Redo();
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(restored_graph_item->GetDataItem(), restored_data_item);
}

//! Create multilayer, add two layers, remove everything and undo. Toy models are used here.
TEST_F(ApplicationModelUndoTests, MultiLayer)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  // creating multi layer and two layers
  auto parent = m_model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();
  auto layer0 = m_model.InsertItem<mvvm::test::toyitems::LayerItem>(parent);
  auto layer1 = m_model.InsertItem<mvvm::test::toyitems::LayerItem>(parent);

  // saving identifiers for further reference
  auto id_parent = parent->GetIdentifier();
  auto id_layer0 = layer0->GetIdentifier();
  auto id_layer1 = layer1->GetIdentifier();

  // checking status of unddo stack
  EXPECT_EQ(commands->GetCommandCount(), 3);
  EXPECT_EQ(commands->GetIndex(), 3);

  // removing multi layer completely
  m_model.RemoveItem(parent);
  EXPECT_EQ(commands->GetCommandCount(), 4);
  EXPECT_EQ(commands->GetIndex(), 4);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // multilayer and its two layers should gone from registration
  EXPECT_EQ(m_pool->ItemForKey(id_parent), nullptr);
  EXPECT_EQ(m_pool->ItemForKey(id_layer0), nullptr);
  EXPECT_EQ(m_pool->ItemForKey(id_layer1), nullptr);

  // undoing multilayer removal
  commands->Undo();
  EXPECT_EQ(commands->GetCommandCount(), 4);
  EXPECT_EQ(commands->GetIndex(), 3);

  // restoring pointers back
  auto parent_at = utils::ChildAt(m_model.GetRootItem(), 0);
  auto layer0_at = utils::ChildAt(parent_at, 0);
  auto layer1_at = utils::ChildAt(parent_at, 1);

  // checking that restored item has corrrect identifiers
  EXPECT_EQ(parent_at->GetIdentifier(), id_parent);
  EXPECT_EQ(layer0_at->GetIdentifier(), id_layer0);
  EXPECT_EQ(layer1_at->GetIdentifier(), id_layer1);

  // checking tag
  const std::string expected_tag("Layers");  // hardcoede in MultiLayerItem
  EXPECT_EQ(layer0_at->GetTagIndex().GetTag(), expected_tag);
  EXPECT_EQ(layer1_at->GetTagIndex().GetTag(), expected_tag);
  std::vector<SessionItem*> expected = {layer0_at, layer1_at};
  EXPECT_EQ(parent_at->GetItems(expected_tag), expected);
}

//! Move layer between two multi layers.
TEST_F(ApplicationModelUndoTests, MoveLayer)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  // creating multi layer and two layers
  auto parent0 = m_model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();
  auto layer0 = m_model.InsertItem<mvvm::test::toyitems::LayerItem>(parent0);
  auto parent1 = m_model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();

  // checking status of unddo stack
  EXPECT_EQ(commands->GetCommandCount(), 3);
  EXPECT_EQ(commands->GetIndex(), 3);

  m_model.MoveItem(layer0, parent1, TagIndex::Append());

  // one command thanks to command macro invokation in SessionModel::MoveItem
  EXPECT_EQ(commands->GetCommandCount(), 4);
  EXPECT_EQ(commands->GetIndex(), 4);

  EXPECT_EQ(parent0->GetLayers().size(), 0);
  EXPECT_EQ(parent1->GetLayers().size(), 1);

  commands->Undo();

  EXPECT_EQ(parent0->GetLayers().size(), 1);
  EXPECT_EQ(parent1->GetLayers().size(), 0);
  EXPECT_EQ(commands->GetCommandCount(), 4);
  EXPECT_EQ(commands->GetIndex(), 3);
}

//! Checking that ReplaceRootItem commands cleans-up the command stack
TEST_F(ApplicationModelUndoTests, Clear)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  // creating multi layer and two layers
  auto parent = m_model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();

  EXPECT_EQ(commands->GetCommandCount(), 1);
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());

  m_model.Clear();

  EXPECT_EQ(commands->GetCommandCount(), 0);
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
}

TEST_F(ApplicationModelUndoTests, DISABLED_UndoSetVectorCoordinate)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  auto vector_item = m_model.InsertItem<VectorItem>();

  EXPECT_NO_THROW(vector_item->SetX(42.0)); // <-- failing here

  // it is failing because VectorItem::Activate tries to update label (and thus update he model)
  // before SetX() execution has finished
  // FIXME enable test after ModelComposerRefactoring in COA-1389
}
