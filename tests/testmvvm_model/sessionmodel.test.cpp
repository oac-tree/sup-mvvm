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

#include "mvvm/core/exceptions.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/model/itempool.h"
#include "mvvm/model/itemutils.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taginfo.h"

#include <gtest/gtest.h>

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
};

TEST_F(SessionModelTest, InitialState)
{
  SessionModel model;
  EXPECT_EQ(model.GetRootItem()->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetParent(), nullptr);
}

TEST_F(SessionModelTest, InsertItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

  const std::string model_type = SessionItem::Type;

  // inserting single item
  auto item = model.InsertItem<SessionItem>();
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->GetParent(), model.GetRootItem());
  EXPECT_EQ(item->GetModel(), &model);
  EXPECT_EQ(item->GetType(), model_type);

  // checking registration
  auto item_key = item->GetIdentifier();
  EXPECT_EQ(pool->ItemForKey(item_key), item);

  // registering tag
  item->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // adding child to it
  auto child = model.InsertItem<SessionItem>(item);
  auto child_key = child->GetIdentifier();
  EXPECT_EQ(pool->ItemForKey(child_key), child);

  EXPECT_TRUE(child != nullptr);
  EXPECT_EQ(child->GetParent(), item);
  EXPECT_EQ(child->GetModel(), &model);
  EXPECT_EQ(child->GetType(), model_type);

  // taking child back
  auto taken = item->TakeItem({"", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(child->GetModel(), nullptr);

  // childitem not registered anymore
  EXPECT_EQ(pool->ItemForKey(child_key), nullptr);
}

//! Inserting single PropertyItem using move.

TEST_F(SessionModelTest, InsertItemViaMove)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

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
  EXPECT_EQ(pool->ItemForKey(item_key), inserted);
}

TEST_F(SessionModelTest, InsertItemWithTag)
{
  const std::string tag1("tag1");
  SessionModel model;
  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  auto child1 = model.InsertItem<PropertyItem>(parent, {tag1, -1});

  EXPECT_EQ(parent->TagIndexOfItem(child1).tag, tag1);
  EXPECT_EQ(utils::IndexOfChild(parent, child1), 0);

  // adding second child
  auto child2 = model.InsertItem<PropertyItem>(parent, {tag1, 0});

  EXPECT_EQ(parent->TagIndexOfItem(child2).tag, tag1);
  EXPECT_EQ(utils::IndexOfChild(parent, child1), 1);
  EXPECT_EQ(utils::IndexOfChild(parent, child2), 0);
}

//! Inserting single PropertyItem using move.

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

TEST_F(SessionModelTest, InsertNewItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

  const std::string model_type = SessionItem::Type;

  // inserting single item
  auto item = model.InsertNewItem(model_type);
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->GetParent(), model.GetRootItem());
  EXPECT_EQ(item->GetModel(), &model);
  EXPECT_EQ(item->GetType(), model_type);

  // checking registration
  auto item_key = item->GetIdentifier();
  EXPECT_EQ(pool->ItemForKey(item_key), item);

  // registering tag
  item->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // adding child to it
  auto child = model.InsertNewItem(model_type, item);
  auto child_key = child->GetIdentifier();
  EXPECT_EQ(pool->ItemForKey(child_key), child);

  EXPECT_TRUE(child != nullptr);
  EXPECT_EQ(child->GetParent(), item);
  EXPECT_EQ(child->GetModel(), &model);
  EXPECT_EQ(child->GetType(), model_type);

  // taking child back
  auto taken = item->TakeItem({"", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(child->GetModel(), nullptr);

  // childitem not registered anymore
  EXPECT_EQ(pool->ItemForKey(child_key), nullptr);
}

TEST_F(SessionModelTest, SetData)
{
  SessionModel model;

  // inserting single item
  auto item = model.InsertItem<SessionItem>();
  EXPECT_TRUE(utils::IsValid(model.Data(item, DataRole::kDisplay)));

  // setting wrong type of data
  variant_t value(42.0);
  EXPECT_THROW(model.SetData(item, value, DataRole::kDisplay), std::runtime_error);

  // setting new data
  EXPECT_TRUE(model.SetData(item, value, DataRole::kData));
  EXPECT_EQ(model.Data(item, DataRole::kData), value);

  // setting same data twice should return false
  EXPECT_FALSE(model.SetData(item, value, DataRole::kData));
}

TEST_F(SessionModelTest, RemoveItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent, {"", 0});  // before child1

  // removing child2
  model.RemoveItem(child2);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::ChildAt(parent, 0), child1);

  // child2 shouldn't be registered anymore
  EXPECT_EQ(pool->KeyForItem(child2), "");
}

TEST_F(SessionModelTest, TakeItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent, {"", 0});  // before child1

  // removing child2
  auto taken = model.TakeItem(parent, {"", 0});  // removing child2
  EXPECT_EQ(taken.get(), child2);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::ChildAt(parent, 0), child1);

  // child2 shouldn't be registered anymore
  EXPECT_EQ(pool->KeyForItem(child2), "");
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
  SessionModel model("Test", pool);

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // removing non existing child
  EXPECT_NO_THROW(model.TakeItem(parent, {"", 0}));
}

TEST_F(SessionModelTest, TakeRowFromRootItem)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("Test", pool);

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto parent_key = parent->GetIdentifier();

  auto child = model.InsertItem<SessionItem>(parent);
  auto child_key = child->GetIdentifier();

  EXPECT_EQ(pool->ItemForKey(parent_key), parent);
  EXPECT_EQ(pool->ItemForKey(child_key), child);

  // taking parent
  auto taken = model.GetRootItem()->TakeItem({"", 0});
  EXPECT_EQ(pool->ItemForKey(parent_key), nullptr);
  EXPECT_EQ(pool->ItemForKey(child_key), nullptr);
}

// !Simple move of item from one parent to another.

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

//! More similar test scenarious in validateutils.test.cpp

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
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("test", pool);

  EXPECT_EQ(pool->GetSize(), 1);

  auto first_root = model.GetRootItem();

  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);
  model.InsertItem<SessionItem>();
  model.InsertItem<SessionItem>();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 2);

  model.Clear();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_FALSE(model.GetRootItem() == first_root);
  EXPECT_EQ(pool->KeyForItem(first_root), "");
  EXPECT_EQ(pool->GetSize(), 1);
}

TEST_F(SessionModelTest, ClearRebuildModel)
{
  auto pool = std::make_shared<ItemPool>();
  SessionModel model("test", pool);

  EXPECT_EQ(pool->GetSize(), 1);

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
  EXPECT_EQ(pool->KeyForItem(first_root), "");
  EXPECT_EQ(pool->GetSize(), 2);
  EXPECT_EQ(pool->KeyForItem(new_item), new_item->GetIdentifier());
}

// FIXME restore copyModelItemRootContext
////! Tests item copy when from root item to root item.

// TEST_F(SessionModelTest, copyModelItemRootContext)
//{
//    SessionModel model;

//    // create single item with value
//    auto item = model.insertItem<SessionItem>();
//    item->setData(42.0);

//    // copying to root item
//    auto copy = model.copyItem(item, model.rootItem());

//    // checking copy
//    ASSERT_TRUE(copy != nullptr);
//    ASSERT_TRUE(copy != item);
//    EXPECT_FALSE(copy->identifier().empty());
//    EXPECT_TRUE(copy->identifier() != item->identifier());
//    EXPECT_EQ(copy->data<double>(), 42.0);
//    EXPECT_EQ(model.rootItem()->children().size(), 2);
//    EXPECT_TRUE(item != copy);
//    std::vector<SessionItem*> expected = {item, copy};
//    EXPECT_EQ(model.rootItem()->children(), expected);
//}

// FIXME restore
////! Tests item copy from parent to root item.

// TEST_F(SessionModelTest, copyParentWithProperty)
//{
//    SessionModel model;

//    // parent with single child and data on ite
//    auto parent0 = model.insertItem<SessionItem>();
//    parent0->registerTag(TagInfo::universalTag("defaultTag"), /*set_as_default*/ true);
//    auto child0 = model.insertItem<SessionItem>(parent0);
//    child0->setData(42.0);

//    // copying whole parent to root
//    auto copy = model.copyItem(parent0, model.rootItem());
//    auto copy_child = copy->getItem("defaultTag");

//    ASSERT_TRUE(copy != nullptr);
//    ASSERT_TRUE(copy_child != nullptr);
//    EXPECT_FALSE(copy->identifier().empty());
//    EXPECT_TRUE(copy->identifier() != parent0->identifier());
//    EXPECT_EQ(copy_child->data<double>(), 42.0);
//}

// FIXME restore
////! Tests item copy for property item.

// TEST_F(SessionModelTest, copyFreeItem)
//{
//    SessionModel model;

//    // single parent in a model
//    auto parent0 = model.insertItem<SessionItem>();
//    parent0->registerTag(TagInfo::universalTag("defaultTag"), /*set_as_default*/ true);

//    // free item
//    auto item = std::make_unique<PropertyItem>();
//    item->setData(42.0);

//    // copying to parent
//    auto copy = model.copyItem(item.get(), parent0);
//    EXPECT_EQ(copy->data<double>(), 42.0);
//}

// FIXME restore forbiddenCopy
////! Attempt to copy property item into the same tag.

// TEST_F(SessionModelTest, forbiddenCopy)
//{
//    SessionModel model;

//    // single parent in a model
//    auto parent0 = model.insertItem<SessionItem>();
//    parent0->registerTag(TagInfo::propertyTag("property", "Property"));
//    auto property = model.insertItem<PropertyItem>(parent0, "property");

//    // copying property to same property tag is not allowed
//    EXPECT_THROW(model.copyItem(property, parent0, {"property", -1}), std::runtime_error);
//    EXPECT_EQ(parent0->childrenCount(), 1);
//}

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
  SessionModel model1("Test1", pool);
  SessionModel model2("Test2", pool);

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

TEST_F(SessionModelTest, GetTopItem)
{
  SessionModel model;
  EXPECT_EQ(model.GetTopItem<>(), nullptr);
  EXPECT_EQ(model.GetTopItem(), nullptr);

  auto property = model.InsertItem<PropertyItem>();
  auto compound = model.InsertItem<CompoundItem>();
  EXPECT_EQ(model.GetTopItem<>(), property);
  EXPECT_EQ(model.GetTopItem(), property);
  EXPECT_EQ(model.GetTopItem<CompoundItem>(), compound);
}

TEST_F(SessionModelTest, GetTopItems)
{
  std::vector<SessionItem*> expected;

  SessionModel model;
  EXPECT_EQ(model.GetTopItems<>(), expected);
  EXPECT_EQ(model.GetTopItems(), expected);

  auto property1 = model.InsertItem<PropertyItem>();
  auto compound1 = model.InsertItem<CompoundItem>();
  auto property2 = model.InsertItem<PropertyItem>();
  auto compound2 = model.InsertItem<CompoundItem>();

  expected = {property1, compound1, property2, compound2};
  EXPECT_EQ(model.GetTopItems<>(), expected);
  EXPECT_EQ(model.GetTopItems(), expected);

  std::vector<CompoundItem*> expected2 = {compound1, compound2};
  EXPECT_EQ(model.GetTopItems<CompoundItem>(), expected2);
}

TEST_F(SessionModelTest, RegisterItem)
{
  const std::string expectedItemType("TestItemType");

  SessionModel model;
  model.RegisterItem<TestItem>();

  auto item = model.InsertNewItem(expectedItemType);
  ASSERT_TRUE(item != nullptr);
  ASSERT_TRUE(dynamic_cast<TestItem*>(item) != nullptr);
  EXPECT_EQ(item->GetType(), expectedItemType);
}
