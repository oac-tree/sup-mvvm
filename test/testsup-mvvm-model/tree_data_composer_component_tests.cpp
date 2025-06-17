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

#include "mvvm/serialization/tree_data_composer_component.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for TreeDataComposerComponent class.
 */
class TreeDataComposerComponentTests : public ::testing::Test
{
};

TEST_F(TreeDataComposerComponentTests, InitialState)
{
  const std::string node_name("node_name");

  const TreeDataComposerComponent component(node_name);

  EXPECT_EQ(component.GetQueuedComponentsCount(), 0U);
  EXPECT_FALSE(component.HasNextComponent());

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), node_name);
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0U);
}

TEST_F(TreeDataComposerComponentTests, AddChildAndAttribute)
{
  const std::string node_name("node_name");
  const std::string attribute_name("attribute_name");
  const std::string attribute_value("attribute_value");

  TreeDataComposerComponent component(node_name);

  const tree_data_t child_tree_data("child_node");
  component.AddChild(child_tree_data);

  component.AddAttribute(attribute_name, attribute_value);

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), node_name);
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(attribute_name), attribute_value);
}

TEST_F(TreeDataComposerComponentTests, AddNextComponent)
{
  const std::string node_name("node_name");

  TreeDataComposerComponent component(node_name);

  const std::string queued_component_name("queued_component_name");
  auto queued_component = std::make_unique<TreeDataComposerComponent>(queued_component_name);
  auto queued_component_ptr = queued_component.get();

  component.AddNextComponent(std::move(queued_component));

  EXPECT_EQ(component.GetQueuedComponentsCount(), 1U);
  EXPECT_TRUE(component.HasNextComponent());

  // moving queued component
  auto taken = component.TakeNextComponent();

  EXPECT_EQ(taken.get(), queued_component_ptr);

  EXPECT_EQ(component.GetQueuedComponentsCount(), 0U);
  EXPECT_FALSE(component.HasNextComponent());
}

TEST_F(TreeDataComposerComponentTests, AddTwoComponents)
{
  const std::string node_name("node_name");

  TreeDataComposerComponent component(node_name);

  const std::string queued_component_name0("queued_component_name0");
  auto queued_component0 = std::make_unique<TreeDataComposerComponent>(queued_component_name0);
  auto queued_component0_ptr = queued_component0.get();

  const std::string queued_component_name1("queued_component_name1");
  auto queued_component1 = std::make_unique<TreeDataComposerComponent>(queued_component_name1);
  auto queued_component1_ptr = queued_component1.get();

  component.AddNextComponent(std::move(queued_component0));
  component.AddNextComponent(std::move(queued_component1));

  EXPECT_EQ(component.GetQueuedComponentsCount(), 2U);
  EXPECT_TRUE(component.HasNextComponent());

  // moving queued component from the front of the queue
  auto taken0 = component.TakeNextComponent();
  auto taken1 = component.TakeNextComponent();

  EXPECT_EQ(taken0.get(), queued_component0_ptr);
  EXPECT_EQ(taken1.get(), queued_component1_ptr);

  EXPECT_EQ(component.GetQueuedComponentsCount(), 0U);
  EXPECT_FALSE(component.HasNextComponent());
}


TEST_F(TreeDataComposerComponentTests, MoveTreeData)
{
  const std::string node_name("node_name");
  const std::string attribute_name("attribute_name");
  const std::string attribute_value("attribute_value");

  TreeDataComposerComponent component(node_name);

  const tree_data_t child_tree_data("child_node");
  component.AddChild(child_tree_data);

  component.AddAttribute(attribute_name, attribute_value);

  const auto tree_data = component.MoveTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), node_name);
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(attribute_name), attribute_value);
}

}  // namespace mvvm::test
