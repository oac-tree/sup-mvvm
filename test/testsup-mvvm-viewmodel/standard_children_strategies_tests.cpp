/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/standard_children_strategies.h"

#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

class StandardChildrenStrategiesTest : public ::testing::Test
{
public:
  //! Helper class with two properties and one top level item on board.
  class TestItem : public CompoundItem
  {
  public:
    TestItem() : CompoundItem("test")
    {
      AddProperty("length", 8.0);
      RegisterTag(TagInfo::CreateUniversalTag("children"), /*set_as_default*/ true);
      InsertItem<SessionItem>(TagIndex::Append());
      AddProperty("height", 12.0);
    }
  };

  struct ChildrenData
  {
    std::string model_type;
    std::string tag;
    bool operator==(const ChildrenData& other) const
    {
      return model_type == other.model_type && tag == other.tag;
    }
  };

  std::vector<ChildrenData> children_data(std::vector<SessionItem*> children)
  {
    std::vector<ChildrenData> result;
    for (auto child : children)
    {
      result.push_back({child->GetType(), child->GetTagIndex().tag});
    }
    return result;
  }
};

//! Testing AllChildrenStrategy.

TEST_F(StandardChildrenStrategiesTest, AllChildrenStrategy)
{
  AllChildrenStrategy strategy;

  // nullptr
  auto children = strategy.GetChildren(nullptr);
  EXPECT_EQ(children.size(), 0);

  // empty item
  SessionItem item1;
  children = strategy.GetChildren(&item1);
  EXPECT_EQ(children.size(), 0);

  // VectorItem
  VectorItem item2;
  children = strategy.GetChildren(&item2);
  EXPECT_EQ(children.size(), 3);

  // CompoundItem
  CompoundItem item3;
  item3.AddProperty("height", 42.0);
  children = strategy.GetChildren(&item3);
  EXPECT_EQ(children.size(), 1);

  // TestItem
  TestItem item4;
  children = strategy.GetChildren(&item4);
  EXPECT_EQ(children.size(), 3);
}

//! Testing AllChildrenStrategy when one of children is hidden.
//! By the current convention this strategy still show all items.

TEST_F(StandardChildrenStrategiesTest, AllChildrenStrategyWhenHidden)
{
  AllChildrenStrategy strategy;
  VectorItem item;
  item.GetItem(VectorItem::kX)->SetVisible(false);
  auto children = strategy.GetChildren(&item);
  EXPECT_EQ(children.size(), 3);  // hidden items are still shown
}

//! Testing TopItemsStrategy.

TEST_F(StandardChildrenStrategiesTest, TopItemsStrategy)
{
  TopItemsStrategy strategy;

  // nullptr
  auto children = strategy.GetChildren(nullptr);
  EXPECT_EQ(children.size(), 0);

  // empty item
  SessionItem item1;
  children = strategy.GetChildren(&item1);
  EXPECT_EQ(children.size(), 0);

  // VectorItem
  VectorItem item2;
  children = strategy.GetChildren(&item2);
  EXPECT_EQ(children.size(), 0);

  // CompoundItem
  CompoundItem item3;
  item3.AddProperty("height", 42.0);
  children = strategy.GetChildren(&item3);
  EXPECT_EQ(children.size(), 0);

  // TestItem
  TestItem item4;
  children = strategy.GetChildren(&item4);
  EXPECT_EQ(children.size(), 1);
}

//! Testing TopItemsStrategy when some items are hidden.

TEST_F(StandardChildrenStrategiesTest, TopItemsStrategyWhenHidden)
{
  TopItemsStrategy strategy;

  SessionModel model;
  auto vec1 = model.InsertItem<VectorItem>();
  auto vec2 = model.InsertItem<VectorItem>();
  vec2->SetVisible(false);
  auto vec3 = model.InsertItem<VectorItem>();

  auto children = strategy.GetChildren(model.GetRootItem());
  EXPECT_EQ(children, std::vector<SessionItem*>({vec1, vec3}));
}

//! Testing PropertyItemsStrategy.

TEST_F(StandardChildrenStrategiesTest, PropertyItemsStrategy)
{
  PropertyItemsStrategy strategy;

  // nullptr
  {
    auto children = strategy.GetChildren(nullptr);
    EXPECT_EQ(children.size(), 0);
  }

  // empty item
  {
    SessionItem item;
    auto children = strategy.GetChildren(&item);
    EXPECT_EQ(children.size(), 0);
  }

  // VectorItem
  {
    VectorItem item;
    auto children = strategy.GetChildren(&item);
    EXPECT_EQ(children.size(), 3);
  }

  // CompoundItem
  {
    CompoundItem item;
    item.AddProperty("height", 42.0);
    auto children = strategy.GetChildren(&item);
    EXPECT_EQ(children.size(), 1);
  }

  // TestItem
  {
    TestItem item;
    auto children = strategy.GetChildren(&item);
    EXPECT_EQ(children.size(), 2);
  }

  // GroupItem FIXME restore ShapeGroupItem
  //  {
  //    ToyItems::ShapeGroupItem item;
  //    item.setCurrentType(ToyItems::Constants::CylinderItemType);
  //    auto children = strategy.children(&item);
  //    EXPECT_EQ(children.size(), 2);

  //    std::vector<ChildrenData> expected_children_data{
  //        {Constants::PropertyType, ToyItems::CylinderItem::P_RADIUS},
  //        {Constants::PropertyType, ToyItems::CylinderItem::P_HEIGHT}};
  //    EXPECT_EQ(children_data(children), expected_children_data);
  //  }
}

//! Testing PropertyItemsStrategy when some items are hidden.

TEST_F(StandardChildrenStrategiesTest, PropertyItemsStrategyWhenHidden)
{
  PropertyItemsStrategy strategy;

  // VectorItem
  {
    VectorItem item;
    item.GetItem(VectorItem::kY)->SetVisible(false);
    auto children = strategy.GetChildren(&item);
    EXPECT_EQ(children.size(), 2);
  }
}
