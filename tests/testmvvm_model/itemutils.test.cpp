/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/itemutils.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/standarditems/vectoritem.h"

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

class ItemUtilsTest : public ::testing::Test
{
};

//! Simple iteration over item and its children

TEST_F(ItemUtilsTest, IterateItem)
{
  std::vector<const SessionItem*> visited_items;

  auto fun = [&](const SessionItem* item) { visited_items.push_back(item); };

  // iteration over nullptr
  Utils::iterate(nullptr, fun);
  EXPECT_TRUE(visited_items.empty());

  // iteration over lonely parent
  std::unique_ptr<SessionItem> parent(new SessionItem);
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  std::vector<const SessionItem*> expected = {parent.get()};
  Utils::iterate(parent.get(), fun);
  EXPECT_EQ(visited_items, expected);

  // adding children
  auto child1 = parent->InsertItem<SessionItem>(TagIndex::Append());
  auto child2 = parent->InsertItem<SessionItem>(TagIndex::Append());

  visited_items.clear();
  Utils::iterate(parent.get(), fun);

  expected = {parent.get(), child1, child2};
  EXPECT_EQ(visited_items, expected);
}

//! Conditional iteration over item and its children.

TEST_F(ItemUtilsTest, IterateIfItem)
{
  std::vector<const SessionItem*> visited_items;

  // function which will not let iterate over children
  std::function<bool(const SessionItem*)> fun = [&](const SessionItem* item)
  {
    visited_items.push_back(item);
    return false;
  };

  // iteration over lonely parent
  std::unique_ptr<SessionItem> parent(new SessionItem);
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  parent->InsertItem<SessionItem>(TagIndex::Append());
  parent->InsertItem<SessionItem>(TagIndex::Append());

  std::vector<const SessionItem*> expected = {parent.get()};
  Utils::iterate_if(parent.get(), fun);
  EXPECT_EQ(visited_items, expected);
}

//! Iteration over root item of the model. FIXME uncomment

TEST_F(ItemUtilsTest, iterateModel)
{
  SessionModel model;

  // building model
  auto parent1 = model.InsertItem<SessionItem>();
  auto parent2 = model.InsertItem<SessionItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  parent2->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child1 = model.InsertItem<SessionItem>(parent1);
  auto child2 = model.InsertItem<SessionItem>(parent1);

  std::vector<const SessionItem*> visited_items;
  auto fun = [&](const SessionItem* item) { visited_items.push_back(item); };

  // iteration
  Utils::iterate(model.GetRootItem(), fun);

  std::vector<const SessionItem*> expected = {model.GetRootItem(), parent1, child1, child2,
                                              parent2};
  EXPECT_EQ(visited_items, expected);
}

//! Copy number of child in parents tree.

TEST_F(ItemUtilsTest, itemCopyNumber)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent);
  auto child3 = model.InsertItem<PropertyItem>(parent);

  EXPECT_EQ(Utils::CopyNumber(child1), 0);
  EXPECT_EQ(Utils::CopyNumber(child2), 1);
  EXPECT_EQ(Utils::CopyNumber(child3), -1);
}

//! Checks method ::HasTag.

TEST_F(ItemUtilsTest, HasTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  EXPECT_TRUE(Utils::HasTag(item, "default_tag"));
  EXPECT_FALSE(Utils::HasTag(item, "nonexisting_tag"));
}

//! Checks method ::IsSinglePropertyTag.

TEST_F(ItemUtilsTest, IsSinglePropertyTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  item.RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  EXPECT_FALSE(Utils::IsSinglePropertyTag(item, "default_tag"));
  EXPECT_TRUE(Utils::IsSinglePropertyTag(item, "property_tag"));
}

//! Checks method ::RegisteredTags.

TEST_F(ItemUtilsTest, RegisteredTags)
{
  SessionItem item;
  EXPECT_TRUE(Utils::RegisteredTags(item).empty());

  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  item.RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  EXPECT_EQ(Utils::RegisteredTags(item), std::vector<std::string>({"default_tag", "property_tag"}));
}

//! Checks method ::RegisteredUniversalTags.

TEST_F(ItemUtilsTest, RegisteredUniversalTags)
{
  SessionItem item;
  EXPECT_TRUE(Utils::RegisteredUniversalTags(item).empty());

  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  item.RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  EXPECT_EQ(Utils::RegisteredUniversalTags(item), std::vector<std::string>({"default_tag"}));
}

//! Check access to top level and property items.

TEST_F(ItemUtilsTest, TopLevelItems)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child3 = model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::TopLevelItems(*model.GetRootItem()), std::vector<SessionItem*>({parent}));
  EXPECT_EQ(Utils::TopLevelItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::TopLevelItems(*parent), std::vector<SessionItem*>({child1, child3}));
}

//! Check access to top level and property items when some of items are hidden via corresponding
//! appearance flag.

TEST_F(ItemUtilsTest, TopLevelItemsWhenHidden)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag1"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag2"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag1");
  auto child2 = model.InsertItem<SessionItem>(parent, "default_tag1");
  child2->SetVisible(false);
  auto child3 = model.InsertItem<SessionItem>(parent, "default_tag2");
  model.InsertItem<PropertyItem>(parent, "property_tag");

  EXPECT_EQ(Utils::TopLevelItems(*model.GetRootItem()), std::vector<SessionItem*>({parent}));
  EXPECT_EQ(Utils::TopLevelItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::TopLevelItems(*parent), std::vector<SessionItem*>({child1, child3}));
}

//! Check access to top level and property items.

TEST_F(ItemUtilsTest, SinglePropertyItems)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<PropertyItem>(parent, "property_tag");
  model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({child2}));
}

//! Check access to top level and property items when some of items are hidden via corresponding
//! appearance flag.

TEST_F(ItemUtilsTest, SinglePropertyItemsWhenHidden)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag1", PropertyItem::Type));
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag2", PropertyItem::Type));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<PropertyItem>(parent, "property_tag1");
  child2->SetVisible(false);
  auto child3 = model.InsertItem<PropertyItem>(parent, "property_tag2");
  model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(Utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({child3}));
}

//! Looking for next item.

TEST_F(ItemUtilsTest, FindNextSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreateUniversalTag("property_tag", {PropertyItem::Type}));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::FindNextSibling(child0), child1);
  EXPECT_EQ(Utils::FindNextSibling(child1), child2);
  EXPECT_EQ(Utils::FindNextSibling(child2), nullptr);
  EXPECT_EQ(Utils::FindNextSibling(property), nullptr);
  EXPECT_EQ(Utils::FindNextSibling(parent), nullptr);
}

//! Looking for previous item.

TEST_F(ItemUtilsTest, FindPreviousSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::FindPreviousSibling(child0), nullptr);
  EXPECT_EQ(Utils::FindPreviousSibling(child1), child0);
  EXPECT_EQ(Utils::FindPreviousSibling(child2), child1);
  EXPECT_EQ(Utils::FindPreviousSibling(property), nullptr);
  EXPECT_EQ(Utils::FindPreviousSibling(parent), nullptr);
}

//! Looking for previous item.

TEST_F(ItemUtilsTest, FindNextItemToSelect)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(Utils::FindNextItemToSelect(child0), child1);
  EXPECT_EQ(Utils::FindNextItemToSelect(child1), child2);
  EXPECT_EQ(Utils::FindNextItemToSelect(child2), child1);
  EXPECT_EQ(Utils::FindNextItemToSelect(property), parent);
  EXPECT_EQ(Utils::FindNextItemToSelect(parent), model.GetRootItem());
}

//! Checking IsItemAncestor.

TEST_F(ItemUtilsTest, IsItemAncestor)
{
  SessionModel model;
  EXPECT_FALSE(Utils::IsItemAncestor(model.GetRootItem(), model.GetRootItem()));

  // rootItem in ancestor of vectorItem, but not vice versa
  auto vector_item = model.InsertItem<VectorItem>();
  EXPECT_TRUE(Utils::IsItemAncestor(vector_item, model.GetRootItem()));
  EXPECT_FALSE(Utils::IsItemAncestor(model.GetRootItem(), vector_item));
  EXPECT_FALSE(Utils::IsItemAncestor(vector_item, vector_item));

  auto x_item = vector_item->GetItem(VectorItem::P_X);

  EXPECT_TRUE(Utils::IsItemAncestor(x_item, model.GetRootItem()));
  EXPECT_TRUE(Utils::IsItemAncestor(x_item, vector_item));
  EXPECT_FALSE(Utils::IsItemAncestor(model.GetRootItem(), x_item));
  EXPECT_FALSE(Utils::IsItemAncestor(vector_item, x_item));

  auto y_item = vector_item->GetItem(VectorItem::P_Y);
  EXPECT_FALSE(Utils::IsItemAncestor(x_item, y_item));
}

TEST_F(ItemUtilsTest, UniqueItems)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());
  std::vector<SessionItem*> data = {nullptr, item0, item1, item2, item0, item2, nullptr};
  std::vector<SessionItem*> expected = {item0, item1, item2};
  EXPECT_EQ(Utils::UniqueItems(data), expected);
}

TEST_F(ItemUtilsTest, CastedItems)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item1 = model.InsertItem<PropertyItem>(model.GetRootItem());
  auto item2 = model.InsertItem<VectorItem>(model.GetRootItem());
  std::vector<SessionItem*> data = {nullptr, item0, item1, item2, item0, item1, item2, nullptr};

  EXPECT_EQ(Utils::CastedItems<PropertyItem>(data), std::vector<PropertyItem*>({item1, item1}));
}
