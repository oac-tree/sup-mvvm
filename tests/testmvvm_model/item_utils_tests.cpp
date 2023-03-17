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

#include "mvvm/model/item_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

class ItemUtilsTests : public ::testing::Test
{
};

//! Simple iteration over item and its children

TEST_F(ItemUtilsTests, IterateItem)
{
  std::vector<const SessionItem*> visited_items;

  auto fun = [&](const SessionItem* item) { visited_items.push_back(item); };

  // iteration over nullptr
  utils::iterate(nullptr, fun);
  EXPECT_TRUE(visited_items.empty());

  // iteration over lonely parent
  std::unique_ptr<SessionItem> parent(new SessionItem);
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  std::vector<const SessionItem*> expected = {parent.get()};
  utils::iterate(parent.get(), fun);
  EXPECT_EQ(visited_items, expected);

  // adding children
  auto child1 = parent->InsertItem<SessionItem>(TagIndex::Append());
  auto child2 = parent->InsertItem<SessionItem>(TagIndex::Append());

  visited_items.clear();
  utils::iterate(parent.get(), fun);

  expected = {parent.get(), child1, child2};
  EXPECT_EQ(visited_items, expected);
}

//! Conditional iteration over item and its children.

TEST_F(ItemUtilsTests, IterateIfItem)
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
  utils::iterate_if(parent.get(), fun);
  EXPECT_EQ(visited_items, expected);
}

//! Iteration over root item of the model.

TEST_F(ItemUtilsTests, IterateModel)
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
  utils::iterate(model.GetRootItem(), fun);

  std::vector<const SessionItem*> expected = {model.GetRootItem(), parent1, child1, child2,
                                              parent2};
  EXPECT_EQ(visited_items, expected);
}

//! Copy number of child in parents tree.

TEST_F(ItemUtilsTests, ItemCopyNumber)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent);
  auto child2 = model.InsertItem<SessionItem>(parent);
  auto child3 = model.InsertItem<PropertyItem>(parent);

  EXPECT_EQ(utils::CopyNumber(child1), 0);
  EXPECT_EQ(utils::CopyNumber(child2), 1);
  EXPECT_EQ(utils::CopyNumber(child3), -1);
}

//! Checks method ::HasTag.

TEST_F(ItemUtilsTests, HasTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  EXPECT_TRUE(utils::HasTag(item, "default_tag"));
  EXPECT_FALSE(utils::HasTag(item, "nonexisting_tag"));
}

//! Checks method ::RegisteredTags.

TEST_F(ItemUtilsTests, RegisteredTags)
{
  SessionItem item;
  EXPECT_TRUE(utils::RegisteredTags(item).empty());

  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  item.RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  EXPECT_EQ(utils::RegisteredTags(item), std::vector<std::string>({"default_tag", "property_tag"}));
}

//! Check access to top level and property items.

TEST_F(ItemUtilsTests, TopLevelItems)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child2 = parent->AddProperty("thickness", 42.0);
  auto child3 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));

  EXPECT_EQ(utils::TopLevelItems(*model.GetRootItem()), std::vector<SessionItem*>({parent}));
  EXPECT_EQ(utils::TopLevelItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::TopLevelItems(*parent), std::vector<SessionItem*>({child1, child3}));
}

//! Check access to top level and property items when some of items are hidden via corresponding
//! appearance flag.

TEST_F(ItemUtilsTests, TopLevelItemsWhenHidden)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag1"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag2"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag1");
  auto child2 = model.InsertItem<SessionItem>(parent, "default_tag1");
  child2->SetVisible(false);
  auto child3 = model.InsertItem<SessionItem>(parent, "default_tag2");

  auto child4 = parent->AddProperty("thickness", 42.0);

  EXPECT_EQ(utils::TopLevelItems(*model.GetRootItem()), std::vector<SessionItem*>({parent}));
  EXPECT_EQ(utils::TopLevelItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::TopLevelItems(*parent), std::vector<SessionItem*>({child1, child3}));
}

//! Check access to top level and property items.

TEST_F(ItemUtilsTests, SinglePropertyItems)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child3 = parent->AddProperty<VectorItem>("position");
  auto child4 = parent->AddProperty("thickness", 42.0);

  model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({child3, child4}));
}

//! Check access to top level and property items when some of items are hidden via corresponding
//! appearance flag.

TEST_F(ItemUtilsTests, SinglePropertyItemsWhenHidden)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = parent->AddProperty<VectorItem>("position");
  child2->SetVisible(false);
  auto child3 = parent->AddProperty("thickness", 42.0);

  EXPECT_EQ(utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({child3}));
}

//! Looking for next item.

TEST_F(ItemUtilsTests, FindNextSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreateUniversalTag("property_tag", {PropertyItem::Type}));

  auto property = model.InsertItem<PropertyItem>(parent, TagIndex::Append("property_tag"));
  auto child0 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child2 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));

  EXPECT_EQ(utils::FindNextSibling(child0), child1);
  EXPECT_EQ(utils::FindNextSibling(child1), child2);
  EXPECT_EQ(utils::FindNextSibling(child2), nullptr);
  EXPECT_EQ(utils::FindNextSibling(property), nullptr);
  EXPECT_EQ(utils::FindNextSibling(parent), nullptr);
}

//! Looking for previous item.

TEST_F(ItemUtilsTests, FindPreviousSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child2 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));

  EXPECT_EQ(utils::FindPreviousSibling(child0), nullptr);
  EXPECT_EQ(utils::FindPreviousSibling(child1), child0);
  EXPECT_EQ(utils::FindPreviousSibling(child2), child1);
  EXPECT_EQ(utils::FindPreviousSibling(property), nullptr);
  EXPECT_EQ(utils::FindPreviousSibling(parent), nullptr);
}

//! Looking for previous item.

TEST_F(ItemUtilsTests, FindNextItemToSelect)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child2 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));

  EXPECT_EQ(utils::FindNextItemToSelect(child0), child1);
  EXPECT_EQ(utils::FindNextItemToSelect(child1), child2);
  EXPECT_EQ(utils::FindNextItemToSelect(child2), child1);
  EXPECT_EQ(utils::FindNextItemToSelect(property), parent);
  EXPECT_EQ(utils::FindNextItemToSelect(parent), model.GetRootItem());
}

//! Checking IsItemAncestor.

TEST_F(ItemUtilsTests, IsItemAncestor)
{
  SessionModel model;
  EXPECT_FALSE(utils::IsItemAncestor(model.GetRootItem(), model.GetRootItem()));

  // rootItem in ancestor of vectorItem, but not vice versa
  auto vector_item = model.InsertItem<VectorItem>();
  EXPECT_TRUE(utils::IsItemAncestor(vector_item, model.GetRootItem()));
  EXPECT_FALSE(utils::IsItemAncestor(model.GetRootItem(), vector_item));
  EXPECT_FALSE(utils::IsItemAncestor(vector_item, vector_item));

  auto x_item = vector_item->GetItem(VectorItem::kX);

  EXPECT_TRUE(utils::IsItemAncestor(x_item, model.GetRootItem()));
  EXPECT_TRUE(utils::IsItemAncestor(x_item, vector_item));
  EXPECT_FALSE(utils::IsItemAncestor(model.GetRootItem(), x_item));
  EXPECT_FALSE(utils::IsItemAncestor(vector_item, x_item));

  auto y_item = vector_item->GetItem(VectorItem::kY);
  EXPECT_FALSE(utils::IsItemAncestor(x_item, y_item));
}

TEST_F(ItemUtilsTests, UniqueItems)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item2 = model.InsertItem<SessionItem>(model.GetRootItem());
  std::vector<SessionItem*> data = {nullptr, item0, item1, item2, item0, item2, nullptr};
  std::vector<SessionItem*> expected = {item0, item1, item2};
  EXPECT_EQ(utils::UniqueItems(data), expected);
}

TEST_F(ItemUtilsTests, CastedItems)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item1 = model.InsertItem<PropertyItem>(model.GetRootItem());
  auto item2 = model.InsertItem<VectorItem>(model.GetRootItem());
  std::vector<SessionItem*> data = {nullptr, item0, item1, item2, item0, item1, item2, nullptr};

  EXPECT_EQ(utils::CastItems<PropertyItem>(data), std::vector<PropertyItem*>({item1, item1}));
}

TEST_F(ItemUtilsTests, GetNestlingDepth)
{
  using mvvm::utils::GetNestlingDepth;
  EXPECT_EQ(GetNestlingDepth(nullptr, nullptr), -1);

  SessionItem item0;
  EXPECT_EQ(GetNestlingDepth(&item0, &item0), 0);

  SessionModel model;
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());
  EXPECT_EQ(GetNestlingDepth(model.GetRootItem(), model.GetRootItem()), 0);
  EXPECT_EQ(GetNestlingDepth(model.GetRootItem(), item1), 1);
  EXPECT_EQ(GetNestlingDepth(item1, model.GetRootItem()), -1);

  auto grand_parent = model.InsertItem<CompoundItem>();
  grand_parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto parent = model.InsertItem<CompoundItem>(grand_parent);
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto child = model.InsertItem<SessionItem>(parent);

  EXPECT_EQ(GetNestlingDepth(child, child), 0);
  EXPECT_EQ(GetNestlingDepth(parent, child), 1);
  EXPECT_EQ(GetNestlingDepth(grand_parent, child), 2);
  EXPECT_EQ(GetNestlingDepth(grand_parent, parent), 1);
  EXPECT_EQ(GetNestlingDepth(child, parent), -1);
}

TEST_F(ItemUtilsTests, HasAppearanceFlag)
{
  using mvvm::utils::HasAppearanceFlag;

  {  // by default item has no appearance flags
    SessionItem item;
    EXPECT_FALSE(HasAppearanceFlag(item, kDisabled));
    EXPECT_FALSE(HasAppearanceFlag(item, kReadOnly));
    EXPECT_FALSE(HasAppearanceFlag(item, kHidden));
    EXPECT_FALSE(HasAppearanceFlag(item, kProperty));
  }

  {
    SessionItem item;
    item.SetEnabled(false);
    EXPECT_TRUE(HasAppearanceFlag(item, kDisabled));
    EXPECT_FALSE(HasAppearanceFlag(item, kReadOnly));
    EXPECT_FALSE(HasAppearanceFlag(item, kHidden));
    EXPECT_FALSE(HasAppearanceFlag(item, kProperty));
    item.SetEnabled(true);
    EXPECT_FALSE(HasAppearanceFlag(item, kDisabled));
    EXPECT_FALSE(HasAppearanceFlag(item, kReadOnly));
    EXPECT_FALSE(HasAppearanceFlag(item, kHidden));
    EXPECT_FALSE(HasAppearanceFlag(item, kProperty));
  }

  {
    SessionItem item;
    item.SetFlag(kProperty, true);
    EXPECT_FALSE(HasAppearanceFlag(item, kDisabled));
    EXPECT_FALSE(HasAppearanceFlag(item, kReadOnly));
    EXPECT_FALSE(HasAppearanceFlag(item, kHidden));
    EXPECT_TRUE(HasAppearanceFlag(item, kProperty));
    item.SetFlag(kProperty, false);
    EXPECT_FALSE(HasAppearanceFlag(item, kDisabled));
    EXPECT_FALSE(HasAppearanceFlag(item, kReadOnly));
    EXPECT_FALSE(HasAppearanceFlag(item, kHidden));
    EXPECT_FALSE(HasAppearanceFlag(item, kProperty));
  }
}

TEST_F(ItemUtilsTests, ReplaceData)
{
  SessionItem item;
  item.SetData(42, DataRole::kData);

  EXPECT_TRUE(utils::ReplaceData(item, std::string("abc"), DataRole::kData));

  EXPECT_EQ(item.Data(), variant_t(std::string("abc")));
}

//! Testing utility function CloneItem.

TEST_F(ItemUtilsTests, CloneItem)
{
  {  // attempt to clone an item that doesn't have a model
    PropertyItem item;
    item.SetData(42.0);

    EXPECT_THROW(utils::CloneItem(item), InvalidOperationException);
  }

  {  // cloning item in board of the model
    SessionModel model;
    auto item = model.InsertItem<PropertyItem>();
    item->SetData(42);

    auto clone = utils::CloneItem(*item);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_EQ(item->GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(item->Data(), clone->Data());
  }
}

//! Testing utility function CopyItem.
//! The difference with previous class is that identifier of copied item shouldn't coincide.

TEST_F(ItemUtilsTests, CopyItem)
{
  {  // attempt to copy an item that doesn't have a model
    PropertyItem item;
    item.SetData(42.0);

    EXPECT_THROW(utils::CopyItem(item), InvalidOperationException);
  }

  {  // cloning item in board of the model
    SessionModel model;
    auto item = model.InsertItem<PropertyItem>();
    item->SetData(42);

    auto clone = utils::CopyItem(*item);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_NE(item->GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(item->Data(), clone->Data());
  }
}
