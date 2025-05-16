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

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>
#include <testutils/model_test_helper.h>

using namespace mvvm;

/**
 * @brief Testing test helpers from model_test_helper.h
 */
class ModelTestHelperTests : public ::testing::Test
{
};

TEST_F(ModelTestHelperTests, CreateTestDataForSingleLevel)
{
  const std::size_t max_children_count{2};
  const std::size_t depth{1};

  auto data = test::CreateTestData(depth, max_children_count);
  auto model = data->model.get();
  EXPECT_EQ(model->GetRootItem()->GetTotalItemCount(), 0);

  ASSERT_EQ(data->item_info.size(), 1);

  auto info = data->item_info.at(0);

  EXPECT_EQ(info.item, model->GetRootItem());
  EXPECT_EQ(info.identifier, model->GetRootItem()->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, info.path), model->GetRootItem());
  EXPECT_EQ(model->FindItem(info.identifier), model->GetRootItem());
}

TEST_F(ModelTestHelperTests, CreateTestDataForTwoLevels)
{
  const std::size_t max_children_count{2};
  const std::size_t depth{2};

  auto data = test::CreateTestData(depth, max_children_count);
  auto model = data->model.get();

  EXPECT_EQ(model->GetRootItem()->GetTotalItemCount(), max_children_count);

  ASSERT_EQ(data->item_info.size(), 3);

  auto root_info = data->item_info.at(0);
  auto child0_info = data->item_info.at(1);
  auto child1_info = data->item_info.at(2);

  EXPECT_EQ(model->GetRootItem()->GetAllItems(),
            std::vector<SessionItem*>({child0_info.item, child1_info.item}));

  auto child0 = model->GetRootItem()->GetAllItems().at(0);
  auto child1 = model->GetRootItem()->GetAllItems().at(1);

  EXPECT_EQ(child0_info.identifier, child0->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, child0_info.path), child0);
  EXPECT_EQ(model->FindItem(child0_info.identifier), child0);

  EXPECT_EQ(child1_info.identifier, child1->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, child1_info.path), child1);
  EXPECT_EQ(model->FindItem(child1_info.identifier), child1);
}

TEST_F(ModelTestHelperTests, CreateTestDataForThreeLevels)
{
  const std::size_t max_children_count{2};
  const std::size_t depth{3};

  auto data = test::CreateTestData(depth, max_children_count);
  auto model = data->model.get();

  EXPECT_EQ(model->GetRootItem()->GetTotalItemCount(), max_children_count);

  ASSERT_EQ(data->item_info.size(), 7);

  auto root_info = data->item_info.at(0);
  auto child0_info = data->item_info.at(1);
  auto child1_info = data->item_info.at(2);

  auto child0 = model->GetRootItem()->GetAllItems().at(0);
  auto child1 = model->GetRootItem()->GetAllItems().at(1);

  EXPECT_EQ(child0_info.identifier, child0->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, child0_info.path), child0);
  EXPECT_EQ(model->FindItem(child0_info.identifier), child0);

  EXPECT_EQ(child1_info.identifier, child1->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, child1_info.path), child1);
  EXPECT_EQ(model->FindItem(child1_info.identifier), child1);

  auto grand_child0_info = data->item_info.at(3);
  auto grand_child1_info = data->item_info.at(4);
  auto grand_child2_info = data->item_info.at(5);
  auto grand_child3_info = data->item_info.at(6);

  EXPECT_EQ(child0->GetAllItems(),
            std::vector<SessionItem*>({grand_child0_info.item, grand_child1_info.item}));
  EXPECT_EQ(child1->GetAllItems(),
            std::vector<SessionItem*>({grand_child2_info.item, grand_child3_info.item}));

  auto grandchild0 = child0->GetAllItems().at(0);
  auto grandchild3 = child1->GetAllItems().at(1);

  EXPECT_EQ(grand_child0_info.identifier, grandchild0->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, grand_child0_info.path), grandchild0);
  EXPECT_EQ(model->FindItem(grand_child0_info.identifier), grandchild0);

  EXPECT_EQ(grand_child3_info.identifier, grandchild3->GetIdentifier());
  EXPECT_EQ(utils::ItemFromPath(*model, grand_child3_info.path), grandchild3);
  EXPECT_EQ(model->FindItem(grand_child3_info.identifier), grandchild3);
}
