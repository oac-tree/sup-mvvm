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

#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>

#include <memory>
#include <stdexcept>

using namespace mvvm;

class SessionModelTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem() : SessionItem("TestItemType"){};
  };

  class TestModel : public SessionModel
  {
  public:
    TestModel() : SessionModel("TestModelType", CreateDefaultItemManager(m_pool)){};
  };

  static std::shared_ptr<ItemPool> m_pool;
};

std::shared_ptr<ItemPool> SessionModelTest::m_pool = std::make_shared<ItemPool>();

TEST_F(SessionModelTest, InitialState)
{
  SessionModel model;
  EXPECT_EQ(model.GetRootItem()->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetParent(), nullptr);
  EXPECT_EQ(model.GetSubscriber(), nullptr);
}

TEST_F(SessionModelTest, SetData)
{
  SessionModel model;

  // inserting single item
  auto item = model.InsertItem<SessionItem>();
  EXPECT_TRUE(utils::IsValid(item->Data(DataRole::kDisplay)));

  // setting wrong type of data
  variant_t value(42.0);
  EXPECT_THROW(model.SetData(item, value, DataRole::kDisplay), RuntimeException);

  // setting new data
  EXPECT_TRUE(model.SetData(item, value, DataRole::kData));
  EXPECT_EQ(item->Data(DataRole::kData), value);

  // setting same data twice should return false
  EXPECT_FALSE(model.SetData(item, value, DataRole::kData));
}

//! Insert item into root.

TEST_F(SessionModelTest, InsertItemIntoRoot)
{
  TestModel model;

  // inserting single item
  auto item = model.InsertItem<SessionItem>();
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->GetParent(), model.GetRootItem());
  EXPECT_EQ(item->GetModel(), &model);
  EXPECT_EQ(item->GetType(), SessionItem::Type);

  // checking registration
  auto item_key = item->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(item_key), item);
}

//! Inserting item into root via move.

TEST_F(SessionModelTest, InsertItemIntoRootViaMove)
{
  TestModel model;

  auto item = std::make_unique<PropertyItem>();
  auto item_ptr = item.get();

  // inserting single item
  auto inserted = model.InsertItem(std::move(item), model.GetRootItem(), {"", 0});

  EXPECT_TRUE(item == nullptr);
  EXPECT_EQ(inserted, item_ptr);
  EXPECT_EQ(inserted->GetParent(), model.GetRootItem());
  EXPECT_EQ(inserted->GetModel(), &model);

  // checking registration
  auto item_key = inserted->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(item_key), inserted);
}

//! Insert item into parent using tag and index.

TEST_F(SessionModelTest, InsertItemIntoParentUsingTagAndIndex)
{
  TestModel model;

  // inserting single item and registering tag
  auto item = model.InsertItem<SessionItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag"), /*set_as_default*/ false);

  // checking item
  auto item_key = item->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(item_key), item);
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->GetParent(), model.GetRootItem());
  EXPECT_EQ(item->GetModel(), &model);
  EXPECT_EQ(item->GetType(), SessionItem::Type);

  // adding child to it
  auto child = model.InsertItem<PropertyItem>(item, {"tag", 0});

  // checking child
  auto child_key = child->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(child_key), child);
  EXPECT_TRUE(child != nullptr);
  EXPECT_EQ(child->GetParent(), item);
  EXPECT_EQ(child->GetModel(), &model);
  EXPECT_EQ(child->GetType(), PropertyItem::Type);

  // adding second child to it

  auto child2 = model.InsertItem<PropertyItem>(item, {"tag", 0});

  EXPECT_EQ(item->TagIndexOfItem(child2).tag, "tag");
  EXPECT_EQ(utils::IndexOfChild(item, child), 1);
  EXPECT_EQ(utils::IndexOfChild(item, child2), 0);
}

//! Insert item into default tag.

TEST_F(SessionModelTest, InsertItemInDefaultTag)
{
  TestModel model;

  // inserting single item and registering tag
  auto item = model.InsertItem<SessionItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag"), /*set_as_default*/ true);

  // adding child to it
  auto child = model.InsertItem<PropertyItem>(item);

  // checking child
  auto child_key = child->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(child_key), child);
  EXPECT_TRUE(child != nullptr);
  EXPECT_EQ(child->GetParent(), item);
  EXPECT_EQ(child->GetModel(), &model);
  EXPECT_EQ(child->GetType(), PropertyItem::Type);
  auto child_tagindex = child->GetTagIndex();
  EXPECT_EQ(child_tagindex.tag, "tag");
  EXPECT_EQ(child_tagindex.index, 0);

  // adding second child to it
  auto child2 = model.InsertItem<PropertyItem>(item);
  auto child2_tagindex = child2->GetTagIndex();
  EXPECT_EQ(child2_tagindex.tag, "tag");
  EXPECT_EQ(child2_tagindex.index, 1);
}

//! Insert item into default tag.

TEST_F(SessionModelTest, InsertItemInDefaultTagViaAppend)
{
  TestModel model;

  // inserting single item and registering tag
  auto item = model.InsertItem<SessionItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag"), /*set_as_default*/ true);

  // adding child to it
  auto child = model.InsertItem<PropertyItem>(item, TagIndex::Append());

  // checking child
  auto child_key = child->GetIdentifier();
  EXPECT_EQ(m_pool->ItemForKey(child_key), child);
  EXPECT_TRUE(child != nullptr);
  EXPECT_EQ(child->GetParent(), item);
  EXPECT_EQ(child->GetModel(), &model);
  EXPECT_EQ(child->GetType(), PropertyItem::Type);
  auto child_tagindex = child->GetTagIndex();
  EXPECT_EQ(child_tagindex.tag, "tag");
  EXPECT_EQ(child_tagindex.index, 0);

  // adding second child to it
  auto child2 = model.InsertItem<PropertyItem>(item);
  auto child2_tagindex = child2->GetTagIndex();
  EXPECT_EQ(child2_tagindex.tag, "tag");
  EXPECT_EQ(child2_tagindex.index, 1);
}

//! Attempt to insert item into default tag when where is no default tag.

TEST_F(SessionModelTest, InsertItemInDefaultTagWhenNoDefaultTagIsPresent)
{
  SessionModel model("Test");

  // inserting single item and registering tag
  auto item = model.InsertItem<SessionItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag"), /*set_as_default*/ false);

  // adding child to default tag
  EXPECT_THROW(model.InsertItem<PropertyItem>(item), InvalidInsertException);
}

//! Attempt to insert item in property tag.

TEST_F(SessionModelTest, InsertItemInPropertyTag)
{
  SessionModel model("Test");

  // inserting single item and registering tag
  auto parent = model.InsertItem<CompoundItem>();
  parent->AddProperty("thickness", 42);

  // adding child to default tag
  EXPECT_THROW(model.InsertItem<PropertyItem>(parent, {"thickness", 0}), InvalidInsertException);
}

//! Attempt to insert item in property tag.

TEST_F(SessionModelTest, InsertNewItemInPropertyTag)
{
  SessionModel model("Test");

  // inserting single item and registering tag
  auto parent = model.InsertItem<CompoundItem>();
  parent->AddProperty("thickness", 42);

  // adding child to default tag
  EXPECT_THROW(model.InsertItem<PropertyItem>(parent, {"thickness", 0}), InvalidInsertException);
}

//! Inserting single PropertyItem using move.
//! Revise test.

TEST_F(SessionModelTest, InsertItemIntoParentViaMove)
{
  const std::string tag1("tag1");
  SessionModel model;
  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1), true);
  auto child1 = model.InsertItem<PropertyItem>(parent, {tag1, -1});

  auto item = std::make_unique<PropertyItem>();
  auto item_ptr = item.get();

  // inserting single item
  auto inserted = model.InsertItem(std::move(item), parent, {"", -1});

  EXPECT_TRUE(item == nullptr);
  EXPECT_EQ(inserted, item_ptr);
  EXPECT_EQ(inserted->GetParent(), parent);
  EXPECT_EQ(inserted->GetModel(), &model);
  EXPECT_EQ(inserted->GetTagIndex(), TagIndex(tag1, 1));
}

TEST_F(SessionModelTest, RemoveItem)
{
  TestModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent, {"", 0});  // before child1

  // removing child2
  model.RemoveItem(child2);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::ChildAt(parent, 0), child1);

  // child2 shouldn't be registered anymore
  EXPECT_EQ(m_pool->KeyForItem(child2), "");
}

TEST_F(SessionModelTest, TakeItem)
{
  TestModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent, {"", 0});  // before child1
  auto child2_key = child2->GetIdentifier();

  // removing child2
  auto taken = model.TakeItem(parent, {"", 0});  // removing child2
  EXPECT_EQ(taken.get(), child2);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::ChildAt(parent, 0), child1);
  EXPECT_EQ(taken->GetModel(), nullptr);

  // child2 shouldn't be registered anymore
  EXPECT_EQ(m_pool->KeyForItem(child2), "");
  EXPECT_EQ(m_pool->ItemForKey(child2_key), nullptr);
}

TEST_F(SessionModelTest, RemoveFromWrongParent)
{
  SessionModel model("Test");

  // undefined item
  EXPECT_THROW(model.TakeItem(nullptr, {"", 0}), std::runtime_error);

  // parent non belonging to given model
  SessionItem parent;
  EXPECT_THROW(model.TakeItem(&parent, {"", 0}), std::runtime_error);
}

TEST_F(SessionModelTest, RemoveNonExistingItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", CreateDefaultItemManager(pool));

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // removing non existing child
  EXPECT_NO_THROW(model.TakeItem(parent, {"", 0}));
}

TEST_F(SessionModelTest, TakeRowFromRootItem)
{
  TestModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto parent_key = parent->GetIdentifier();

  auto child = model.InsertItem<SessionItem>(parent);
  auto child_key = child->GetIdentifier();

  EXPECT_EQ(m_pool->ItemForKey(parent_key), parent);
  EXPECT_EQ(m_pool->ItemForKey(child_key), child);

  // taking parent
  auto taken = model.GetRootItem()->TakeItem({"", 0});
  EXPECT_EQ(m_pool->ItemForKey(parent_key), nullptr);
  EXPECT_EQ(m_pool->ItemForKey(child_key), nullptr);
}

//! Simple move of item from one parent to another.

TEST_F(SessionModelTest, MoveItem)
{
  SessionModel model;

  // parent with child
  auto parent0 = model.InsertItem<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child0 = model.InsertItem<PropertyItem>(parent0);

  // another parent with child
  auto parent1 = model.InsertItem<SessionItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child1 = model.InsertItem<PropertyItem>(parent1);

  // moving child0 from parent0 to parent 1
  model.MoveItem(child0, parent1, {"", 0});

  std::vector<SessionItem*> expected = {child0, child1};
  EXPECT_EQ(parent1->GetAllItems(), expected);
  EXPECT_TRUE(parent0->GetAllItems().empty());
}

TEST_F(SessionModelTest, MoveItemRootContextNext)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());  // 0
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());  // 1
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());  // 2
  auto item3 = model.InsertItem<SessionItem>(model.GetRootItem());  // 3

  // moving item1 to the next position
  model.MoveItem(item1, model.GetRootItem(), {"", 2});

  // expecting new order of items
  std::vector<SessionItem*> expected = {item0, item2, item1, item3};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

TEST_F(SessionModelTest, MoveItemRootContextSamePos)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());  // 0
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());  // 1
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());  // 2
  auto item3 = model.InsertItem<SessionItem>(model.GetRootItem());  // 3

  // moving item1 to the same position
  model.MoveItem(item1, model.GetRootItem(), {"", 1});

  // expecting new order of items
  std::vector<SessionItem*> expected = {item0, item1, item2, item3};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

TEST_F(SessionModelTest, MoveItemRootContextPrev)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());  // 0
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());  // 1
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());  // 2
  auto item3 = model.InsertItem<SessionItem>(model.GetRootItem());  // 3

  // moving item2 to item1's place
  model.MoveItem(item2, model.GetRootItem(), {"", 1});

  // expecting new order of items
  std::vector<SessionItem*> expected = {item0, item2, item1, item3};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

TEST_F(SessionModelTest, MoveItemRootContextLast)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());  // 0
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());  // 1
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());  // 2
  auto item3 = model.InsertItem<SessionItem>(model.GetRootItem());  // 3

  // moving item0 in the back of the list
  model.MoveItem(item0, model.GetRootItem(), {"", model.GetRootItem()->GetTotalItemCount() - 1});

  // expecting new order of items
  std::vector<SessionItem*> expected = {item1, item2, item3, item0};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

TEST_F(SessionModelTest, MoveItemRootContextLast2)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());  // 0
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());  // 1
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());  // 2
  auto item3 = model.InsertItem<SessionItem>(model.GetRootItem());  // 3

  // moving item0 in the back of the list
  model.MoveItem(item0, model.GetRootItem(), {"", 3});

  // expecting new order of items
  std::vector<SessionItem*> expected = {item1, item2, item3, item0};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

TEST_F(SessionModelTest, MoveItemFromRootToParent)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto parent = model.InsertItem<SessionItem>(model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child0 = model.InsertItem<SessionItem>(parent);
  auto child1 = model.InsertItem<SessionItem>(parent);

  // moving item0 from root to parent
  model.MoveItem(item0, parent, {"", 1});

  // expected items for root item
  std::vector<SessionItem*> expected = {parent};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);

  // expected items for parent
  expected = {child0, item0, child1};
  EXPECT_EQ(parent->GetAllItems(), expected);
}

TEST_F(SessionModelTest, InvalidMoveOfParentProperty)
{
  SessionModel model;
  auto compound = model.InsertItem<CompoundItem>(model.GetRootItem());
  auto property = compound->AddProperty("thickness", 42);

  auto new_parent = model.InsertItem<SessionItem>(model.GetRootItem());
  new_parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // attempt to move property from parent to root
  EXPECT_THROW(model.MoveItem(property, new_parent, {"", 0}), InvalidMoveException);

  // items as before
  EXPECT_EQ(new_parent->GetTotalItemCount(), 0);
  EXPECT_EQ(compound->GetItem("thickness"), property);
}

//! More similar test scenarious in validate_utils_tests.cpp

TEST_F(SessionModelTest, MoveItemBetweenParentTags)
{
  SessionModel model;
  auto parent = model.InsertItem<SessionItem>(model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"));
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag2"));

  auto child0 = model.InsertItem<SessionItem>(parent, "tag1");
  auto child1 = model.InsertItem<SessionItem>(parent, "tag1");
  auto child2 = model.InsertItem<SessionItem>(parent, "tag2");
  auto child3 = model.InsertItem<SessionItem>(parent, "tag2");

  // moving child2 to another tag
  model.MoveItem(child2, parent, {"tag1", 0});

  // expected items for root item
  std::vector<SessionItem*> expected = {parent};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);

  // expected items for parents tag
  expected = {child2, child0, child1, child3};
  EXPECT_EQ(parent->GetAllItems(), expected);
  expected = {child2, child0, child1};
  EXPECT_EQ(parent->GetItems("tag1"), expected);
  expected = {child3};
  EXPECT_EQ(parent->GetItems("tag2"), expected);
}

TEST_F(SessionModelTest, MoveItemFromParentToRoot)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto parent = model.InsertItem<SessionItem>(model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child0 = model.InsertItem<SessionItem>(parent);
  auto child1 = model.InsertItem<SessionItem>(parent);

  // moving child0 from parent to root
  model.MoveItem(child0, model.GetRootItem(), {"", 0});

  // expected items for root item
  std::vector<SessionItem*> expected = {child0, item0, parent};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);

  // expected items for parent
  expected = {child1};
  EXPECT_EQ(parent->GetAllItems(), expected);
}

TEST_F(SessionModelTest, ClearModel)
{
  TestModel model;

  EXPECT_EQ(m_pool->GetSize(), 1);

  auto first_root = model.GetRootItem();

  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);
  model.InsertItem<SessionItem>();
  model.InsertItem<SessionItem>();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 2);

  model.Clear();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_FALSE(model.GetRootItem() == first_root);
  EXPECT_EQ(m_pool->KeyForItem(first_root), "");
  EXPECT_EQ(m_pool->GetSize(), 1);
}

TEST_F(SessionModelTest, ClearRebuildModel)
{
  TestModel model;

  EXPECT_EQ(m_pool->GetSize(), 1);

  auto first_root = model.GetRootItem();

  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);
  model.InsertItem<SessionItem>();
  model.InsertItem<SessionItem>();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 2);

  SessionItem* new_item{nullptr};
  auto rebuild = [&new_item](auto parent)
  { new_item = parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append()); };

  model.Clear(rebuild);
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_FALSE(model.GetRootItem() == first_root);
  EXPECT_EQ(m_pool->KeyForItem(first_root), "");
  EXPECT_EQ(m_pool->GetSize(), 2);
  EXPECT_EQ(m_pool->KeyForItem(new_item), new_item->GetIdentifier());
}

//! Test item find using identifier.

TEST_F(SessionModelTest, FindItem)
{
  SessionModel model;
  auto parent = model.InsertItem<SessionItem>();

  // check that we can find item using its own identofoer
  const std::string id = parent->GetIdentifier();
  EXPECT_EQ(model.FindItem(id), parent);

  // check that we can't find deleted item.
  model.RemoveItem(parent);
  EXPECT_EQ(model.FindItem(id), nullptr);
}

//! Test items in different models.

TEST_F(SessionModelTest, FindItemInAlienModel)
{
  // two models with common pool
  auto pool = std::make_shared<ItemPool>();
  SessionModel model1("Test1", CreateDefaultItemManager(pool));
  SessionModel model2("Test2", CreateDefaultItemManager(pool));

  // inserting items in both models
  auto parent1 = model1.InsertItem<SessionItem>();
  auto parent2 = model2.InsertItem<SessionItem>();
  const std::string id1 = parent1->GetIdentifier();
  const std::string id2 = parent2->GetIdentifier();

  // checking that we can access items from both models
  EXPECT_EQ(model1.FindItem(id1), parent1);
  EXPECT_EQ(model2.FindItem(id1), parent1);
  EXPECT_EQ(model1.FindItem(id2), parent2);
  EXPECT_EQ(model2.FindItem(id2), parent2);

  // check that we can't find deleted item.
  model1.RemoveItem(parent1);
  EXPECT_EQ(model1.FindItem(id1), nullptr);
  EXPECT_EQ(model2.FindItem(id1), nullptr);
  EXPECT_EQ(model1.FindItem(id2), parent2);
  EXPECT_EQ(model2.FindItem(id2), parent2);
}

TEST_F(SessionModelTest, RegisterItem)
{
  const std::string expectedItemType("TestItemType");

  SessionModel model;
  model.RegisterItem<TestItem>();

  auto item =
      model.InsertItem(model.GetFactory()->CreateItem(expectedItemType), model.GetRootItem(), {});
  ASSERT_TRUE(item != nullptr);
  ASSERT_TRUE(dynamic_cast<TestItem*>(item) != nullptr);
  EXPECT_EQ(item->GetType(), expectedItemType);
}
