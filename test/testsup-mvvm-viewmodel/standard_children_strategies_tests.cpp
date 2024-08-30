/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/test/toy_items.h>

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
};

//! Testing AllChildrenStrategy.
TEST_F(StandardChildrenStrategiesTest, AllChildrenStrategy)
{
  const AllChildrenStrategy strategy;

  {  // nullptr
    EXPECT_EQ(strategy.GetChildren(nullptr).size(), 0);
  }

  {  // empty item
    const SessionItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 0);
  }

  {  // VectorItem
    VectorItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 3);
  }

  {  // CompoundItem
    CompoundItem item;
    item.AddProperty("height", 42.0);
    EXPECT_EQ(strategy.GetChildren(&item).size(), 1);
  }

  {  // TestItem
    TestItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 3);
  }
}

//! Testing AllVisibleChildrenStrategy.
TEST_F(StandardChildrenStrategiesTest, AllVisibleChildrenStrategy)
{
  AllVisibleChildrenStrategy strategy;

  {  // VectorItem
    const VectorItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 3);

    item.GetItem(VectorItem::kY)->SetVisible(false);
    EXPECT_EQ(strategy.GetChildren(&item).size(), 2);
    EXPECT_EQ(
        strategy.GetChildren(&item),
        std::vector<SessionItem*>({item.GetItem(VectorItem::kX), item.GetItem(VectorItem::kZ)}));
  }

  {  // TestItem
    const TestItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 3);

    item.GetItem("children")->SetVisible(false);
    EXPECT_EQ(strategy.GetChildren(&item).size(), 2);
  }
}

//! Testing AllChildrenStrategy when one of children is hidden.
//! By the current convention this strategy still show all items.
TEST_F(StandardChildrenStrategiesTest, AllChildrenStrategyWhenHidden)
{
  const AllChildrenStrategy strategy;
  VectorItem item;
  item.GetItem(VectorItem::kX)->SetVisible(false);
  EXPECT_EQ(strategy.GetChildren(&item).size(), 3);  // hidden items are still shown
}

//! Testing TopItemsStrategy.
TEST_F(StandardChildrenStrategiesTest, TopItemsStrategy)
{
  const TopItemsStrategy strategy;

  {  // nullptr
    EXPECT_EQ(strategy.GetChildren(nullptr).size(), 0);
  }

  {  // empty item
    const SessionItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 0);
  }

  {  // VectorItem
    VectorItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 0);
  }

  {  // CompoundItem
    CompoundItem item;
    item.AddProperty("height", 42.0);
    EXPECT_EQ(strategy.GetChildren(&item).size(), 0);
  }

  {  // TestItem
    TestItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 1);
  }
}

//! Testing TopItemsStrategy when some items are hidden.

TEST_F(StandardChildrenStrategiesTest, TopItemsStrategyWhenHidden)
{
  const TopItemsStrategy strategy;

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
  const PropertyItemsStrategy strategy;

  {  // nullptr
    EXPECT_EQ(strategy.GetChildren(nullptr).size(), 0);
  }

  {  // empty item
    const SessionItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 0);
  }

  {  // VectorItem
    const VectorItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 3);
  }

  {  // CompoundItem
    CompoundItem item;
    item.AddProperty("height", 42.0);
    EXPECT_EQ(strategy.GetChildren(&item).size(), 1);
  }

  {  // TestItem
    const TestItem item;
    EXPECT_EQ(strategy.GetChildren(&item).size(), 2);
  }
}

//! Testing PropertyItemsStrategy when some items are hidden.
TEST_F(StandardChildrenStrategiesTest, PropertyItemsStrategyWhenHidden)
{
  const PropertyItemsStrategy strategy;

  const VectorItem item;
  item.GetItem(VectorItem::kY)->SetVisible(false);
  EXPECT_EQ(strategy.GetChildren(&item).size(), 2);
}

//! Testing FixedItemTypeStrategy
TEST_F(StandardChildrenStrategiesTest, FixedItemTypeStrategy)
{
  test::toyitems::ParticleItem particle_item;
  VectorItem vector_item;

  const FixedItemTypeStrategy strategy({VectorItem::Type});
  EXPECT_TRUE(strategy.GetChildren(nullptr).empty());
  EXPECT_TRUE(strategy.GetChildren(&vector_item).empty());

  ASSERT_EQ(strategy.GetChildren(&particle_item).size(), 1);
  EXPECT_EQ(strategy.GetChildren(&particle_item),
            std::vector<SessionItem*>({particle_item.GetItem("position")}));
}
