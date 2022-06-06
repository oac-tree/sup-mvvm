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

#include "mvvm/model/sessionitem_container.h"

#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for SessionItemContainer class.

class SessionItemContainerTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem(const std::string& model_type) : SessionItem(model_type) {}
  };
};

//! Initial state of emty SessionItemTag.

TEST_F(SessionItemContainerTest, InitialState)
{
  const std::string name("tag");
  SessionItemContainer tag(TagInfo::CreateUniversalTag(name));

  EXPECT_EQ(tag.GetItemCount(), 0);
  EXPECT_TRUE(tag.IsEmpty());
  EXPECT_EQ(tag.GetName(), name);
  EXPECT_EQ(tag.GetItems(), std::vector<SessionItem*>());
}

//! Checking ::insertItem.

TEST_F(SessionItemContainerTest, InsertItem)
{
  const std::string tag_name("tag");
  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // inserting non-existing item is not allowed
  EXPECT_FALSE(tag.InsertItem(nullptr, tag.GetItemCount()));
  EXPECT_EQ(tag.GetItemCount(), 0);

  // insertion at the end
  auto child1 = new SessionItem;
  auto child2 = new SessionItem;
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child2, tag.GetItemCount()));
  EXPECT_EQ(tag.GetItemCount(), 2);
  EXPECT_FALSE(tag.IsEmpty());
  std::vector<SessionItem*> expected = {child1, child2};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion at the beginning
  auto child3 = new SessionItem;
  EXPECT_TRUE(tag.InsertItem(child3, 0));
  expected = {child3, child1, child2};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion in between
  auto child4 = new SessionItem;
  EXPECT_TRUE(tag.InsertItem(child4, 1));
  expected = {child3, child4, child1, child2};
  EXPECT_EQ(tag.GetItems(), expected);

  // using index equal to number of items
  auto child5 = new SessionItem;
  EXPECT_TRUE(tag.InsertItem(child5, tag.GetItemCount()));
  expected = {child3, child4, child1, child2, child5};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion with wrong index
  auto child6 = new SessionItem;
  EXPECT_FALSE(tag.InsertItem(child6, 42));
  EXPECT_EQ(tag.GetItems(), expected);
  delete child6;
}

//! Checking ::insertItem when item has specific model type.

TEST_F(SessionItemContainerTest, InsertItemWithType)
{
  const std::string tag_name("tag");
  const std::vector<std::string> model_types = {"model_a"};

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name, model_types));

  // insertion of wrong model type is not allowed
  auto child1 = new TestItem("model_a");
  auto child2 = new TestItem("model_b");
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_FALSE(tag.InsertItem(child2, tag.GetItemCount()));
  delete child2;

  std::vector<SessionItem*> expected = {child1};
  EXPECT_EQ(tag.GetItems(), expected);
}

//! Checking ::insertItem when tag is related to property tag.

TEST_F(SessionItemContainerTest, InsertItemPropertyType)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // insertion of second property item is not allowed (because of reached maximum)
  auto child1 = new TestItem(property_type);
  auto child2 = new TestItem(property_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_FALSE(tag.InsertItem(child2, tag.GetItemCount()));
  delete child2;

  // insertion of wrong model type is not allowed
  auto child3 = new TestItem("another_model");
  EXPECT_FALSE(tag.InsertItem(child3, tag.GetItemCount()));
  delete child3;
  std::vector<SessionItem*> expected = {child1};
  EXPECT_EQ(tag.GetItems(), expected);
}

//! Checking ::indexOfItem.

TEST_F(SessionItemContainerTest, IndexOfItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // index of two items
  auto child1 = new TestItem(model_type);
  auto child2 = new TestItem(model_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_EQ(tag.IndexOfItem(child1), 0);
  EXPECT_TRUE(tag.InsertItem(child2, tag.GetItemCount()));
  EXPECT_EQ(tag.IndexOfItem(child1), 0);
  EXPECT_EQ(tag.IndexOfItem(child2), 1);

  // not existing items
  EXPECT_EQ(tag.IndexOfItem(nullptr), -1);
  auto child3 = std::make_unique<TestItem>(model_type);
  EXPECT_EQ(tag.IndexOfItem(child3.get()), -1);
}

//! Checking ::itemAt.

TEST_F(SessionItemContainerTest, ItemAt)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // items at given indices
  auto child1 = new TestItem(model_type);
  auto child2 = new TestItem(model_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child2, tag.GetItemCount()));
  EXPECT_EQ(tag.ItemAt(0), child1);
  EXPECT_EQ(tag.ItemAt(1), child2);
  // non-existing indices
  EXPECT_EQ(tag.ItemAt(2), nullptr);
  EXPECT_EQ(tag.ItemAt(3), nullptr);
  EXPECT_EQ(tag.ItemAt(-1), nullptr);
}

//! Checking ::takeItem.

TEST_F(SessionItemContainerTest, TakeItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // taking non existing items
  EXPECT_EQ(tag.TakeItem(0), nullptr);

  // inserting items
  auto child1 = new TestItem(model_type);
  auto child2 = new TestItem(model_type);
  auto child3 = new TestItem(model_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child2, tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child3, tag.GetItemCount()));

  // taking item in between
  auto taken2 = tag.TakeItem(1);
  EXPECT_EQ(child2, taken2);
  delete taken2;

  // order of remaining children
  std::vector<SessionItem*> expected = {child1, child3};
  EXPECT_EQ(tag.GetItems(), expected);

  // taking non existing items
  EXPECT_EQ(tag.TakeItem(-1), nullptr);
  EXPECT_EQ(tag.TakeItem(tag.GetItemCount()), nullptr);
}

//! Checking ::canTakeItem.

TEST_F(SessionItemContainerTest, CanTakeItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem(0));

  // inserting items
  auto child1 = new TestItem(model_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));
  EXPECT_TRUE(tag.CanTakeItem(0));

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem(-1));
  EXPECT_FALSE(tag.CanTakeItem(tag.GetItemCount()));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTest, CanInsertItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // inserting non-existing item
  EXPECT_FALSE(tag.CanInsertItem(nullptr, 0));

  // we should be allowed to insert valid child
  auto child1 = std::make_unique<TestItem>(model_type);
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), 0));

  // wrong index is not allowed for insertion
  EXPECT_FALSE(tag.CanInsertItem(child1.get(), 1));
  EXPECT_FALSE(tag.CanInsertItem(child1.get(), -1));

  // inserting child
  EXPECT_TRUE(tag.InsertItem(child1.release(), tag.GetItemCount()));

  // can we insert second child?
  auto child2 = std::make_unique<TestItem>(model_type);
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), 0));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), 1));
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), 2));
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), -1));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTest, CanInsertItemForPropertyTag)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // we should be allowed to insert valid child
  auto child1 = std::make_unique<TestItem>(property_type);
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), 0));

  // inserting child
  EXPECT_TRUE(tag.InsertItem(child1.release(), tag.GetItemCount()));

  // second property shouldn't be posible to insert because of exceeded maximum
  auto child2 = std::make_unique<TestItem>(property_type);
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), 0));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTest, CanInsertItemForUniversalTag)
{
  const std::string tag1 = "tag1";
  const int max_items = 2;
  auto parent = std::make_unique<SessionItem>();
  SessionItemContainer tag(TagInfo(tag1, 0, max_items, std::vector<std::string>() = {}));

  // inserting child
  auto child1 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child1.get(), -1));  // implementation requires exact index
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), 0));
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child1.release(), tag.GetItemCount()));

  // inserting second child
  auto child2 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), -1));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), 0));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), tag.GetItemCount()));
  EXPECT_TRUE(tag.InsertItem(child2.release(), tag.GetItemCount()));

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), -1));
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), 0));
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), tag.GetItemCount()));
}

//! Checking ::takeItem when tag is related to property tag.

TEST_F(SessionItemContainerTest, TakeItemPropertyType)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // insertion of second property item is not allowed (because of reached maximum)
  auto child1 = new TestItem(property_type);
  EXPECT_TRUE(tag.InsertItem(child1, tag.GetItemCount()));

  // attempt to take property item
  EXPECT_FALSE(tag.CanTakeItem(0));
  EXPECT_EQ(tag.TakeItem(0), nullptr);
}
