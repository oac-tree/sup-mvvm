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

#include "mvvm/model/sessionitem_container.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/test/test_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for SessionItemContainer class.

class SessionItemContainerTests : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    explicit TestItem(const std::string& model_type) : SessionItem(model_type) {}
  };

  template <typename T = SessionItem, typename... Args>
  static auto CreateItem(Args&&... args)
  {
    return mvvm::test::CreateTestData<T>(std::forward<Args>(args)...);
  }
};

//! Initial state of emty SessionItemTag.

TEST_F(SessionItemContainerTests, InitialState)
{
  const std::string name("tag");
  const SessionItemContainer tag(TagInfo::CreateUniversalTag(name));

  EXPECT_EQ(tag.GetItemCount(), 0);
  EXPECT_TRUE(tag.IsEmpty());
  EXPECT_EQ(tag.GetName(), name);
  EXPECT_EQ(tag.GetItems(), std::vector<SessionItem*>());
}

//! Checking ::insertItem.

TEST_F(SessionItemContainerTests, InsertItem)
{
  const std::string tag_name("tag");
  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // inserting non-existing item is not allowed
  EXPECT_EQ(tag.InsertItem(nullptr, tag.GetItemCount()), nullptr);
  EXPECT_EQ(tag.GetItemCount(), 0);

  // insertion at the end
  auto [child1, child1_ptr] = CreateItem();
  auto [child2, child2_ptr] = CreateItem();
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), child2_ptr);
  EXPECT_EQ(tag.GetItemCount(), 2);
  EXPECT_FALSE(tag.IsEmpty());
  std::vector<SessionItem*> expected = {child1_ptr, child2_ptr};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion at the beginning
  auto [child3, child3_ptr] = CreateItem();
  EXPECT_EQ(tag.InsertItem(std::move(child3), 0), child3_ptr);
  expected = {child3_ptr, child1_ptr, child2_ptr};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion in between
  auto [child4, child4_ptr] = CreateItem();
  EXPECT_EQ(tag.InsertItem(std::move(child4), 1), child4_ptr);
  expected = {child3_ptr, child4_ptr, child1_ptr, child2_ptr};
  EXPECT_EQ(tag.GetItems(), expected);

  // using index equal to number of items
  auto [child5, child5_ptr] = CreateItem();
  EXPECT_EQ(tag.InsertItem(std::move(child5), tag.GetItemCount()), child5_ptr);
  expected = {child3_ptr, child4_ptr, child1_ptr, child2_ptr, child5_ptr};
  EXPECT_EQ(tag.GetItems(), expected);

  // insertion with wrong index
  auto [child6, child6_ptr] = CreateItem();
  EXPECT_EQ(tag.InsertItem(std::move(child6), 42), nullptr);
  EXPECT_EQ(tag.GetItems(), expected);
}

//! Checking ::insertItem when item has specific model type.

TEST_F(SessionItemContainerTests, InsertItemWithType)
{
  const std::string tag_name("tag");
  const std::vector<std::string> model_types = {"model_a"};

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name, model_types));

  // insertion of wrong model type is not allowed
  auto [child1, child1_ptr] = CreateItem<TestItem>("model_a");
  auto [child2, child2_ptr] = CreateItem<TestItem>("model_b");
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), nullptr);

  EXPECT_EQ(tag.GetItems(), std::vector<SessionItem*>({child1_ptr}));
}

//! Checking ::insertItem when tag is related to property tag.

TEST_F(SessionItemContainerTests, InsertItemPropertyType)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // insertion of second property item is not allowed (because of reached maximum)
  auto [child1, child1_ptr] = CreateItem<TestItem>(property_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(property_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), nullptr);

  // insertion of wrong model type is not allowed
  auto [child3, child3_ptr] = CreateItem<TestItem>("another_model");
  EXPECT_EQ(tag.InsertItem(std::move(child3), tag.GetItemCount()), nullptr);
  EXPECT_EQ(tag.GetItems(), std::vector<SessionItem*>({child1_ptr}));
}

//! Checking ::indexOfItem.

TEST_F(SessionItemContainerTests, IndexOfItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // index of two items
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.IndexOfItem(child1_ptr), 0);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), child2_ptr);
  EXPECT_EQ(tag.IndexOfItem(child1_ptr), 0);
  EXPECT_EQ(tag.IndexOfItem(child2_ptr), 1);

  // not existing items
  EXPECT_EQ(tag.IndexOfItem(nullptr), -1);
  auto child3 = std::make_unique<TestItem>(model_type);
  EXPECT_EQ(tag.IndexOfItem(child3.get()), -1);
}

//! Checking ::itemAt.

TEST_F(SessionItemContainerTests, ItemAt)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // items at given indices
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), child2_ptr);
  EXPECT_EQ(tag.ItemAt(0), child1_ptr);
  EXPECT_EQ(tag.ItemAt(1), child2_ptr);
  // non-existing indices
  EXPECT_EQ(tag.ItemAt(2), nullptr);
  EXPECT_EQ(tag.ItemAt(3), nullptr);
  EXPECT_EQ(tag.ItemAt(-1), nullptr);
}

//! Checking ::takeItem.

TEST_F(SessionItemContainerTests, TakeItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // taking non existing items
  EXPECT_EQ(tag.TakeItem(0), nullptr);

  // inserting items
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(model_type);
  auto [child3, child3_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), child2_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child3), tag.GetItemCount()), child3_ptr);

  // taking item in between
  auto taken2 = tag.TakeItem(1);
  EXPECT_EQ(child2_ptr, taken2.get());

  // order of remaining children
  EXPECT_EQ(tag.GetItems(), std::vector<SessionItem*>({child1_ptr, child3_ptr}));

  // taking non existing items
  EXPECT_EQ(tag.TakeItem(-1), nullptr);
  EXPECT_EQ(tag.TakeItem(tag.GetItemCount()), nullptr);
}

//! Checking ::canTakeItem.

TEST_F(SessionItemContainerTests, CanTakeItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem(0));

  // inserting items
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);
  EXPECT_TRUE(tag.CanTakeItem(0));

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem(-1));
  EXPECT_FALSE(tag.CanTakeItem(tag.GetItemCount()));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTests, CanInsertItem)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name));

  // inserting non-existing item
  EXPECT_FALSE(tag.CanInsertItem(nullptr, 0));

  // we should be allowed to insert valid child
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_TRUE(tag.CanInsertItem(child1_ptr, 0));

  // wrong index is not allowed for insertion
  EXPECT_FALSE(tag.CanInsertItem(child1_ptr, 1));
  EXPECT_FALSE(tag.CanInsertItem(child1_ptr, -1));

  // inserting child
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);

  // can we insert second child?
  auto child2 = std::make_unique<TestItem>(model_type);
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), 0));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), 1));
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), 2));
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), -1));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTests, CanInsertItemForPropertyTag)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // we should be allowed to insert valid child
  auto [child1, child1_ptr] = CreateItem<TestItem>(property_type);
  EXPECT_TRUE(tag.CanInsertItem(child1_ptr, 0));

  // inserting child
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);

  // second property shouldn't be posible to insert because of exceeded maximum
  auto child2 = std::make_unique<TestItem>(property_type);
  EXPECT_FALSE(tag.CanInsertItem(child2.get(), 0));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTests, CanInsertItemForUniversalTag)
{
  const std::string tag1 = "tag1";
  const int max_items = 2;
  auto parent = std::make_unique<SessionItem>();
  SessionItemContainer tag(TagInfo(tag1, 0, max_items, std::vector<std::string>() = {}));

  // inserting child
  auto [child1, child1_ptr] = CreateItem<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child1_ptr, -1));  // implementation requires exact index
  EXPECT_TRUE(tag.CanInsertItem(child1_ptr, 0));
  EXPECT_TRUE(tag.CanInsertItem(child1_ptr, tag.GetItemCount()));
  EXPECT_EQ(tag.InsertItem(std::move(child1), tag.GetItemCount()), child1_ptr);

  // inserting second child
  auto [child2, child2_ptr] = CreateItem<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child2_ptr, -1));
  EXPECT_TRUE(tag.CanInsertItem(child2_ptr, 0));
  EXPECT_TRUE(tag.CanInsertItem(child2_ptr, tag.GetItemCount()));
  EXPECT_EQ(tag.InsertItem(std::move(child2), tag.GetItemCount()), child2_ptr);

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), -1));
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), 0));
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), tag.GetItemCount()));
}

//! Checking ::canInsertItem for own item. A request to insert own item is triggered by move
//! command.

TEST_F(SessionItemContainerTests, CanInsertOwnItem)
{
  const std::string tag1 = "tag1";
  const int max_items = 2;
  auto parent = std::make_unique<SessionItem>();
  SessionItemContainer tag(TagInfo::CreateUniversalTag("aaa"));

  // inserting child
  auto [child0, child0_ptr] = CreateItem<SessionItem>();
  auto [child1, child1_ptr] = CreateItem<SessionItem>();
  auto [child2, child2_ptr] = CreateItem<SessionItem>();
  auto [child3, child3_ptr] = CreateItem<SessionItem>();
  tag.InsertItem(std::move(child0), 0);
  tag.InsertItem(std::move(child1), 1);
  tag.InsertItem(std::move(child2), 2);
  tag.InsertItem(std::move(child3), 3);

  EXPECT_TRUE(tag.CanInsertItem(child0_ptr, 3));

  // should't be possible to insert at this index, since own item will be first removed and total
  // item count should be adjusted
  EXPECT_FALSE(tag.CanInsertItem(child0_ptr, 4));
}

//! Checking ::canInsertItem.

TEST_F(SessionItemContainerTests, CanInsertType)
{
  const std::string tag_name("tag");
  const std::string model_type("model_a");

  SessionItemContainer tag(TagInfo::CreateUniversalTag(tag_name, {tag_name}));

  // inserting non-existing item
  EXPECT_FALSE(tag.CanInsertType("non-existing", 0));
  EXPECT_TRUE(tag.CanInsertType(tag_name, 0));
  EXPECT_FALSE(tag.CanInsertType(tag_name, 1));
}

//! Checking ::takeItem when tag is related to property tag.

TEST_F(SessionItemContainerTests, TakeItemPropertyType)
{
  const std::string name("tag");
  const std::string property_type("Property");

  SessionItemContainer tag(TagInfo::CreatePropertyTag(name, property_type));

  // insertion of second property item is not allowed (because of reached maximum)
  auto child1 = std::make_unique<TestItem>(property_type);
  EXPECT_TRUE(tag.InsertItem(std::move(child1), tag.GetItemCount()));

  // attempt to take property item
  EXPECT_FALSE(tag.CanTakeItem(0));
  EXPECT_EQ(tag.TakeItem(0), nullptr);
}

TEST_F(SessionItemContainerTests, Clone)
{
  const std::string tag_name("tag");
  SessionItemContainer container(TagInfo::CreateUniversalTag(tag_name));

  auto [child1, child1_ptr] = CreateItem();
  EXPECT_EQ(container.InsertItem(std::move(child1), container.GetItemCount()), child1_ptr);

  {  // deep copy
    auto clone = container.Clone(/*make_unique_id*/ true);
    EXPECT_EQ(clone->GetTagInfo(), container.GetTagInfo());
    ASSERT_EQ(clone->GetItemCount(), 1);
    EXPECT_NE(clone->ItemAt(0), child1_ptr);
    EXPECT_NE(clone->ItemAt(0)->GetIdentifier(), child1_ptr->GetIdentifier());
  }

  {  // clone
    auto clone = container.Clone(/*make_unique_id*/ false);
    EXPECT_EQ(clone->GetTagInfo(), container.GetTagInfo());
    ASSERT_EQ(clone->GetItemCount(), 1);
    EXPECT_NE(clone->ItemAt(0), child1_ptr);
    EXPECT_EQ(clone->ItemAt(0)->GetIdentifier(), child1_ptr->GetIdentifier());
  }
}
