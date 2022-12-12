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

#include "mvvm/model/application_model.h"

#include <gtest/gtest.h>
#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <testutils/mock_model_listener.h>
#include <testutils/toy_items.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelComposer class.
//! Class is used to manipulate the model and generate necessary notifications.

class ApplicationModelUndoTests : public ::testing::Test
{
public:
  ApplicationModelUndoTests() : m_model("TestModelType", CreateDefaultItemManager(m_pool))
  {
    m_model.RegisterItem<testutils::toyitems::LayerItem>();
    m_model.RegisterItem<testutils::toyitems::MultiLayerItem>();
  }

  ApplicationModel m_model;
  static std::shared_ptr<ItemPool> m_pool;
};

std::shared_ptr<ItemPool> ApplicationModelUndoTests::m_pool = std::make_shared<ItemPool>();

//! Inserting item, changing the data, removing item, undoing.

TEST_F(ApplicationModelUndoTests, InsertItemSetDataRemoveItem)
{
  m_model.SetUndoEnabled(true);

  auto commands = m_model.GetCommandStack();

  // inserting item and setting the data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  // status of command stack with two commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetSize(), 2);

  // removing item
  m_model.RemoveItem(item);

  // status of command stack with three commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(commands->GetSize(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undoing last removal
  commands->Undo();

  // status of command stack
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetSize(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(m_model.GetRootItem()->GetItem("", 0)->Data(), variant_t(42));
}

//! Add GraphItem and Data1DItem, addisgn data to graph, undo, then redo.
//! GraphItem should be pointing again to Data1DItem.
//! This is real bug case.

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
  EXPECT_EQ(commands->GetSize(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(graph_item->GetDataItem(), data_item);

  // checking pool
  EXPECT_EQ(m_pool->ItemForKey(data_item_identifier), data_item);
  EXPECT_EQ(m_pool->ItemForKey(graph_item_identifier), graph_item);

  // undoing once (setDataItem operation)
  commands->Undo();

  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetSize(), 3);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(graph_item->GetDataItem(), nullptr);

  // undoing two more times item
  commands->Undo();
  commands->Undo();
  EXPECT_EQ(commands->GetIndex(), 0);
  EXPECT_EQ(commands->GetSize(), 3);
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

//! Create multilayer, add two layers, remove everything and undo.
//! Toy models are used here.

TEST_F(ApplicationModelUndoTests, MultiLayer)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  // creating multi layer and two layers
  auto parent = m_model.InsertItem<testutils::toyitems::MultiLayerItem>();
  auto layer0 = m_model.InsertItem<testutils::toyitems::LayerItem>(parent);
  auto layer1 = m_model.InsertItem<testutils::toyitems::LayerItem>(parent);

  // saving identifiers for further reference
  auto id_parent = parent->GetIdentifier();
  auto id_layer0 = layer0->GetIdentifier();
  auto id_layer1 = layer1->GetIdentifier();

  // checking status of unddo stack
  EXPECT_EQ(commands->GetSize(), 3);
  EXPECT_EQ(commands->GetIndex(), 3);

  // removing multi layer completely
  m_model.RemoveItem(parent);
  EXPECT_EQ(commands->GetSize(), 4);
  EXPECT_EQ(commands->GetIndex(), 4);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // multilayer and its two layers should gone from registration
  EXPECT_EQ(m_pool->ItemForKey(id_parent), nullptr);
  EXPECT_EQ(m_pool->ItemForKey(id_layer0), nullptr);
  EXPECT_EQ(m_pool->ItemForKey(id_layer1), nullptr);

  // undoing multilayer removal
  commands->Undo();
  EXPECT_EQ(commands->GetSize(), 4);
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
  EXPECT_EQ(layer0_at->GetTagIndex().tag, expected_tag);
  EXPECT_EQ(layer1_at->GetTagIndex().tag, expected_tag);
  std::vector<SessionItem*> expected = {layer0_at, layer1_at};
  EXPECT_EQ(parent_at->GetItems(expected_tag), expected);
}

//! Checking that Clear commands cleans-up the command stack

TEST_F(ApplicationModelUndoTests, Clear)
{
  m_model.SetUndoEnabled(true);
  auto commands = m_model.GetCommandStack();

  // creating multi layer and two layers
  auto parent = m_model.InsertItem<testutils::toyitems::MultiLayerItem>();

  EXPECT_EQ(commands->GetSize(), 1);
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());

  m_model.Clear({});

  EXPECT_EQ(commands->GetSize(), 0);
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
}
