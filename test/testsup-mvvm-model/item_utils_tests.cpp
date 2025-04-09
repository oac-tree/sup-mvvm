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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/item_utils.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <memory>

using namespace mvvm;

class ItemUtilsTests : public ::testing::Test
{
};

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
  const SessionItem* const_parent = parent.get();
  utils::iterate(const_parent, fun);
  EXPECT_EQ(visited_items, expected);

  // adding children
  auto child1 = parent->InsertItem<SessionItem>(TagIndex::Append());
  auto child2 = parent->InsertItem<SessionItem>(TagIndex::Append());

  visited_items.clear();
  utils::iterate(const_parent, fun);

  expected = {parent.get(), child1, child2};
  EXPECT_EQ(visited_items, expected);
}

TEST_F(ItemUtilsTests, NonConstIterateItem)
{
  std::vector<SessionItem*> visited_items;

  auto fun = [&](SessionItem* item) { visited_items.push_back(item); };

  // iteration over nullptr
  utils::iterate(nullptr, fun);
  EXPECT_TRUE(visited_items.empty());

  // iteration over lonely parent
  std::unique_ptr<SessionItem> parent(new SessionItem);
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  std::vector<SessionItem*> expected = {parent.get()};
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

  const std::vector<const SessionItem*> expected = {parent.get()};
  utils::iterate_if(parent.get(), fun);
  EXPECT_EQ(visited_items, expected);
}

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

  const std::vector<const SessionItem*> expected = {model.GetRootItem(), parent1, child1, child2,
                                                    parent2};
  EXPECT_EQ(visited_items, expected);
}

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

TEST_F(ItemUtilsTests, HasTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  EXPECT_TRUE(utils::HasTag(item, "default_tag"));
  EXPECT_FALSE(utils::HasTag(item, "nonexisting_tag"));
}

TEST_F(ItemUtilsTests, RegisteredTags)
{
  SessionItem item;
  EXPECT_TRUE(utils::RegisteredTags(item).empty());

  item.RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  item.RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));

  EXPECT_EQ(utils::RegisteredTags(item), std::vector<std::string>({"default_tag", "property_tag"}));
}

TEST_F(ItemUtilsTests, TopLevelItems)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto& child2 = parent->AddProperty("thickness", 42.0);
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

  auto& child4 = parent->AddProperty("thickness", 42.0);

  EXPECT_EQ(utils::TopLevelItems(*model.GetRootItem()), std::vector<SessionItem*>({parent}));
  EXPECT_EQ(utils::TopLevelItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::TopLevelItems(*parent), std::vector<SessionItem*>({child1, child3}));
}
TEST_F(ItemUtilsTests, SinglePropertyItems)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto child2 = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto& child3 = parent->AddProperty<VectorItem>("position");
  auto& child4 = parent->AddProperty("thickness", 42.0);

  model.InsertItem<SessionItem>(parent, "default_tag");

  EXPECT_EQ(utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({&child3, &child4}));
}

//! Check access to the top level and property items when some of items are hidden via corresponding
//! appearance flag.
TEST_F(ItemUtilsTests, SinglePropertyItemsWhenHidden)
{
  SessionModel model;

  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);

  auto child1 = model.InsertItem<SessionItem>(parent, "default_tag");
  auto& child2 = parent->AddProperty<VectorItem>("position");
  child2.SetVisible(false);
  auto& child3 = parent->AddProperty("thickness", 42.0);

  EXPECT_EQ(utils::SinglePropertyItems(*model.GetRootItem()), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*child1), std::vector<SessionItem*>({}));
  EXPECT_EQ(utils::SinglePropertyItems(*parent), std::vector<SessionItem*>({&child3}));
}

TEST_F(ItemUtilsTests, FindNextSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreateUniversalTag("property_tag", {PropertyItem::GetStaticType()}));

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

TEST_F(ItemUtilsTests, FindPreviousSibling)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));

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

TEST_F(ItemUtilsTests, FindNextItemToSelect)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));

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

TEST_F(ItemUtilsTests, FindNextSiblingToSelect)
{
  SessionModel model;

  auto parent = model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));

  auto property = model.InsertItem<PropertyItem>(parent, "property_tag");
  auto child0 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));
  auto child2 = model.InsertItem<SessionItem>(parent, TagIndex::Append("default_tag"));

  EXPECT_EQ(utils::FindNextSiblingToSelect(child0), child1);
  EXPECT_EQ(utils::FindNextSiblingToSelect(child1), child2);
  EXPECT_EQ(utils::FindNextSiblingToSelect(child2), child1);
  EXPECT_EQ(utils::FindNextSiblingToSelect(property), nullptr);
  EXPECT_EQ(utils::FindNextSiblingToSelect(parent), nullptr);
}

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
  const std::vector<SessionItem*> data = {nullptr, item0, item1, item2, item0, item2, nullptr};
  const std::vector<SessionItem*> expected = {item0, item1, item2};
  EXPECT_EQ(utils::UniqueItems(data), expected);
}

TEST_F(ItemUtilsTests, CastedItems)
{
  SessionModel model;
  auto item0 = model.InsertItem<SessionItem>(model.GetRootItem());
  auto item1 = model.InsertItem<PropertyItem>(model.GetRootItem());
  auto item2 = model.InsertItem<VectorItem>(model.GetRootItem());
  const std::vector<SessionItem*> data = {nullptr, item0, item1, item2,
                                          item0,   item1, item2, nullptr};

  EXPECT_EQ(utils::CastItems<PropertyItem>(data), std::vector<PropertyItem*>({item1, item1}));
}

TEST_F(ItemUtilsTests, GetNestingDepthWrtBasis)
{
  using mvvm::utils::GetNestingDepth;
  EXPECT_EQ(GetNestingDepth(nullptr, nullptr), -1);

  const SessionItem item0;
  EXPECT_EQ(GetNestingDepth(&item0, &item0), 0);

  SessionModel model;
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());
  EXPECT_EQ(GetNestingDepth(model.GetRootItem(), model.GetRootItem()), 0);
  EXPECT_EQ(GetNestingDepth(model.GetRootItem(), item1), 1);
  EXPECT_EQ(GetNestingDepth(item1, model.GetRootItem()), -1);

  auto grand_parent = model.InsertItem<CompoundItem>();
  grand_parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto parent = model.InsertItem<CompoundItem>(grand_parent);
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto child = model.InsertItem<SessionItem>(parent);

  EXPECT_EQ(GetNestingDepth(child, child), 0);
  EXPECT_EQ(GetNestingDepth(parent, child), 1);
  EXPECT_EQ(GetNestingDepth(grand_parent, child), 2);
  EXPECT_EQ(GetNestingDepth(grand_parent, parent), 1);
  EXPECT_EQ(GetNestingDepth(child, parent), -1);
}

TEST_F(ItemUtilsTests, GetNestingDepthWrtRootItem)
{
  using mvvm::utils::GetNestingDepth;
  EXPECT_EQ(GetNestingDepth(nullptr), -1);

  const SessionItem item0;
  EXPECT_EQ(GetNestingDepth(&item0), 0);

  SessionModel model;
  auto item1 = model.InsertItem<SessionItem>(model.GetRootItem());
  EXPECT_EQ(GetNestingDepth(model.GetRootItem()), 0);
  EXPECT_EQ(GetNestingDepth(item1), 1);

  auto grand_parent = model.InsertItem<CompoundItem>();
  grand_parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto parent = model.InsertItem<CompoundItem>(grand_parent);
  parent->RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
  auto child = model.InsertItem<SessionItem>(parent);

  EXPECT_EQ(GetNestingDepth(child), 3);
  EXPECT_EQ(GetNestingDepth(parent), 2);
  EXPECT_EQ(GetNestingDepth(grand_parent), 1);
}

TEST_F(ItemUtilsTests, HasAppearanceFlag)
{
  using mvvm::utils::HasAppearanceFlag;

  {  // by default item has no appearance flags
    const SessionItem item;
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

TEST_F(ItemUtilsTests, FindItemUp)
{
  {  // start from parent mode
    SessionModel model;

    auto item0 = model.InsertItem<CompoundItem>(model.GetRootItem());
    item0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

    auto item1 = model.InsertItem<PropertyItem>(item0);
    item1->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

    auto item2 = model.InsertItem<VectorItem>(item1);

    EXPECT_EQ(utils::FindItemUp<VectorItem>(item2, false), nullptr);
    EXPECT_EQ(utils::FindItemUp<PropertyItem>(item2, false), item1);
    EXPECT_EQ(utils::FindItemUp<CompoundItem>(item2, false), item0);

    EXPECT_EQ(utils::FindItemUp<VectorItem>(item1, false), nullptr);
    EXPECT_EQ(utils::FindItemUp<PropertyItem>(item1, false), nullptr);
    EXPECT_EQ(utils::FindItemUp<CompoundItem>(item1, false), item0);

    EXPECT_EQ(utils::FindItemUp<VectorItem>(item0, false), nullptr);
    EXPECT_EQ(utils::FindItemUp<PropertyItem>(item0, false), nullptr);
    EXPECT_EQ(utils::FindItemUp<CompoundItem>(item0, false), nullptr);

    EXPECT_EQ(utils::FindItemUp<SessionItem>(item0, false), model.GetRootItem());
  }

  {  // start from self mode
    EXPECT_EQ(utils::FindItemUp<VectorItem>(nullptr, true), nullptr);

    VectorItem vector_item;
    SessionItem* item = &vector_item;
    EXPECT_EQ(utils::FindItemUp<VectorItem>(item), &vector_item);
    EXPECT_EQ(utils::FindItemUp<VectorItem>(vector_item.GetItem(VectorItem::kX)), &vector_item);

    const test::toyitems::ParticleItem particle;
    auto x_item = particle.GetItem("position")->GetItem(VectorItem::kX);
    EXPECT_EQ(utils::FindItemUp<test::toyitems::ParticleItem>(x_item), &particle);
    EXPECT_EQ(utils::FindItemUp<test::toyitems::LayerItem>(x_item), nullptr);
  }
}

TEST_F(ItemUtilsTests, FindItemDown)
{
  using mvvm::test::toyitems::LayerItem;
  using mvvm::test::toyitems::MultiLayerItem;
  using mvvm::test::toyitems::ParticleItem;

  EXPECT_TRUE(utils::FindItemDown<ParticleItem>(nullptr).empty());

  {  // single particle
    ParticleItem particle;
    const std::vector<const ParticleItem*> expected = {&particle};
    EXPECT_EQ(utils::FindItemDown<ParticleItem>(&particle), expected);

    EXPECT_TRUE(
        utils::FindItemDown<ParticleItem>(&particle, {}, /*start_from_self*/ false).empty());
  }

  {  // multilayer with two layers
    MultiLayerItem multilayer;
    auto layer0 = multilayer.InsertItem<LayerItem>(mvvm::TagIndex::Append());
    auto particle0 = layer0->InsertItem<ParticleItem>(mvvm::TagIndex::Append());
    auto particle1 = layer0->InsertItem<ParticleItem>(mvvm::TagIndex::Append());
    auto layer1 = multilayer.InsertItem<LayerItem>(mvvm::TagIndex::Append());
    auto particle2 = layer1->InsertItem<ParticleItem>(mvvm::TagIndex::Append());

    EXPECT_EQ(utils::FindItemDown<LayerItem>(&multilayer),
              std::vector<const LayerItem*>({layer0, layer1}));
    EXPECT_EQ(utils::FindItemDown<ParticleItem>(&multilayer),
              std::vector<const ParticleItem*>({particle0, particle1, particle2}));
  }

  {  // multilayer with two layers, using check on model type
    MultiLayerItem multilayer;
    auto layer0 = multilayer.InsertItem<LayerItem>(mvvm::TagIndex::Append());
    auto particle0 = layer0->InsertItem<ParticleItem>(mvvm::TagIndex::Append());
    auto particle1 = layer0->InsertItem<ParticleItem>(mvvm::TagIndex::Append());
    auto layer1 = multilayer.InsertItem<LayerItem>(mvvm::TagIndex::Append());
    auto particle2 = layer1->InsertItem<ParticleItem>(mvvm::TagIndex::Append());

    EXPECT_EQ(utils::FindItemDown<SessionItem>(&multilayer, ParticleItem::GetStaticType()),
              std::vector<const SessionItem*>({particle0, particle1, particle2}));

    EXPECT_EQ(utils::FindItemDown(&multilayer, ParticleItem::GetStaticType()),
              std::vector<const SessionItem*>({particle0, particle1, particle2}));
  }
}

TEST_F(ItemUtilsTests, ClonePropertyItem)
{
  PropertyItem property;
  property.SetData(42);

  {  // checking clone, when pointer to SessionItem is used
    SessionItem* to_clone = &property;
    auto clone = utils::CloneItem(*to_clone);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_EQ(property.GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(property.Data(), clone->Data());
  }

  {  // checking when return type was preserved
    std::unique_ptr<PropertyItem> clone = utils::CloneItem(property);
    EXPECT_EQ(property.GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(property.Data(), clone->Data());
  }
}

TEST_F(ItemUtilsTests, CloneParentAndChild)
{
  // creating parent with one child
  auto parent = std::make_unique<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // creating copy
  auto parent_copy = utils::CloneItem(*parent);

  EXPECT_EQ(parent_copy->GetTotalItemCount(), 1);
  EXPECT_EQ(parent_copy->GetDisplayName(), "parent_name");
  EXPECT_EQ(parent_copy->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(parent_copy->GetModel(), nullptr);
  EXPECT_EQ(parent_copy->GetIdentifier(), parent->GetIdentifier());  // same identifiers

  // checking child reconstruction
  auto child_copy = parent_copy->GetItem("defaultTag");
  EXPECT_EQ(child_copy->GetParent(), parent_copy.get());
  EXPECT_EQ(child_copy->GetTotalItemCount(), 0);
  EXPECT_EQ(child_copy->GetDisplayName(), "child_name");
  EXPECT_EQ(child_copy->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(child_copy->GetIdentifier(), child->GetIdentifier());  // same identifiers
}

//! Testing utility function CopyItem for simple PropertyItem. Copied items looks the same as cloned
//! one, except that identifiers are different.
TEST_F(ItemUtilsTests, CopyItem)
{
  PropertyItem property;
  property.SetData(42);

  {  // checking copy, when pointer to SessionItem is used
    SessionItem* to_copy = &property;
    auto clone = utils::CopyItem(*to_copy);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_NE(property.GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(property.Data(), clone->Data());
  }

  {  // checking when return type was preserved
    std::unique_ptr<PropertyItem> copy = utils::CopyItem(property);
    EXPECT_NE(property.GetIdentifier(), copy->GetIdentifier());
    EXPECT_EQ(property.Data(), copy->Data());
  }
}

TEST_F(ItemUtilsTests, CopyParentAndChild)
{
  // creating parent with one child
  auto parent = std::make_unique<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // creating copy
  auto parent_copy = utils::CopyItem(*parent);

  EXPECT_EQ(parent_copy->GetTotalItemCount(), 1);
  EXPECT_EQ(parent_copy->GetDisplayName(), "parent_name");
  EXPECT_EQ(parent_copy->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(parent_copy->GetModel(), nullptr);
  EXPECT_NE(parent_copy->GetIdentifier(), parent->GetIdentifier());  // different identifiers

  // checking child reconstruction
  auto child_copy = parent_copy->GetItem("defaultTag");
  EXPECT_EQ(child_copy->GetParent(), parent_copy.get());
  EXPECT_EQ(child_copy->GetTotalItemCount(), 0);
  EXPECT_EQ(child_copy->GetDisplayName(), "child_name");
  EXPECT_EQ(child_copy->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_NE(child_copy->GetIdentifier(), child->GetIdentifier());  // different identifiers
}

TEST_F(ItemUtilsTests, MoveItemUp)
{
  mvvm::test::toyitems::MultiLayerItem multilayer;

  auto layer0 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());
  auto layer1 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());
  auto layer2 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());

  std::vector<SessionItem*> expected = {layer0, layer1, layer2};

  // original layout
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving top layer up doesn't change the order
  EXPECT_FALSE(utils::MoveUp(*layer0));
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving bottom layer up does change the order
  EXPECT_TRUE(utils::MoveUp(*layer2));
  expected = {layer0, layer2, layer1};
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);
}

TEST_F(ItemUtilsTests, MoveItemUpWhenInModel)
{
  mvvm::test::toyitems::SampleModel model;

  auto multilayer = model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();
  auto layer0 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto layer1 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto layer2 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);

  std::vector<SessionItem*> expected = {layer0, layer1, layer2};

  // original layout
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving top layer up doesn't change the order
  EXPECT_FALSE(utils::MoveUp(*layer0));
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving bottom layer up does change the order
  EXPECT_TRUE(utils::MoveUp(*layer2));
  expected = {layer0, layer2, layer1};
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);
}

TEST_F(ItemUtilsTests, MoveItemDown)
{
  mvvm::test::toyitems::MultiLayerItem multilayer;

  auto layer0 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());
  auto layer1 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());
  auto layer2 = multilayer.InsertItem<mvvm::test::toyitems::LayerItem>(TagIndex::Append());

  std::vector<SessionItem*> expected = {layer0, layer1, layer2};

  // original layout
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving bottom layer down doesn't change the order
  EXPECT_FALSE(utils::MoveDown(*layer2));
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving top layer down changes the order
  EXPECT_TRUE(utils::MoveDown(*layer0));
  expected = {layer1, layer0, layer2};
  EXPECT_EQ(multilayer.GetItems(mvvm::test::toyitems::kLayerTag), expected);
}

TEST_F(ItemUtilsTests, MoveItemDownWhenInModel)
{
  mvvm::test::toyitems::SampleModel model;

  auto multilayer = model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();
  auto layer0 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto layer1 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto layer2 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);

  std::vector<SessionItem*> expected = {layer0, layer1, layer2};

  // original layout
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving bottom layer down doesn't change the order
  EXPECT_FALSE(utils::MoveDown(*layer2));
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);

  // moving top layer down changes the order
  EXPECT_TRUE(utils::MoveDown(*layer0));
  expected = {layer1, layer0, layer2};
  EXPECT_EQ(multilayer->GetItems(mvvm::test::toyitems::kLayerTag), expected);
}

TEST_F(ItemUtilsTests, RemoveItem)
{
  {  // item doesn't belong to a parent
    SessionItem item;
    EXPECT_THROW(utils::RemoveItem(item), LogicErrorException);
  }

  {  // item belong to parent
    CompoundItem parent;
    parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
    auto item = parent.InsertItem<PropertyItem>(TagIndex::Append());

    EXPECT_EQ(parent.GetTotalItemCount(), 1);

    utils::RemoveItem(*item);
    EXPECT_EQ(parent.GetTotalItemCount(), 0);
  }

  // extra test when item is a part of the model is in application_model_tests.cpp
}

TEST_F(ItemUtilsTests, InsertItem)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  auto inserted = utils::InsertItem(std::move(child), parent.get(), TagIndex::Append());

  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem({"", 0}), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());

  // extra test when item is a part of the model is in application_model_tests.cpp
}

TEST_F(ItemUtilsTests, ReplaceItem)
{
  const auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  const auto old_item = parent->InsertItem<SessionItem>(TagIndex::Append());

  auto new_item = std::make_unique<SessionItem>();
  const auto p_new_item = new_item.get();

  const auto replacee =
      utils::ReplaceItem(std::move(new_item), old_item->GetParent(), old_item->GetTagIndex());

  EXPECT_EQ(replacee, p_new_item);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), p_new_item), 0);
  EXPECT_EQ(parent->GetAllItems()[0], p_new_item);
  EXPECT_EQ(parent->GetItem({"", 0}), p_new_item);
  EXPECT_EQ(replacee->GetParent(), parent.get());

  // extra test when item is a part of the model is in application_model_tests.cpp
}

TEST_F(ItemUtilsTests, ToStringAndBackNewID)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // to string
  auto str = utils::ToXMLString(parent);

  // reconstructiong back
  auto reco_parent = utils::SessionItemFromXMLString(str);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_NE(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_NE(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

TEST_F(ItemUtilsTests, CopyToStringWithFilter)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child0 = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child0->SetDisplayName("child_name0");
  auto child1 = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child1->SetDisplayName("child_name1");

  auto filter_func = [](const auto& item) -> bool
  { return item.GetDisplayName() != "child_name0"; };

  // to string
  auto str = utils::ToXMLString(parent, filter_func);

  // reconstructiong back
  auto reco_parent = utils::SessionItemFromXMLString(str);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_NE(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name1");
  EXPECT_NE(reco_child->GetIdentifier(), child1->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

TEST_F(ItemUtilsTests, ToStringAndBackClonedID)
{
  SessionItem parent;
  parent.SetDisplayName("abc");

  // to string
  auto str = utils::ToXMLString(parent);

  // reconstructiong back in full clone mode
  auto reco_parent = utils::SessionItemFromXMLString(str, /*generate new id*/ false);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetDisplayName(), "abc");
}

TEST_F(ItemUtilsTests, FindItemsUp)
{
  mvvm::test::toyitems::SampleModel model;

  auto multilayer = model.InsertItem<mvvm::test::toyitems::MultiLayerItem>();
  auto layer0 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto particle0 = model.InsertItem<mvvm::test::toyitems::ParticleItem>(layer0);
  auto layer1 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);
  auto layer2 = model.InsertItem<mvvm::test::toyitems::LayerItem>(multilayer);

  EXPECT_TRUE(utils::FindItemsUp<mvvm::test::toyitems::LayerItem>({}).empty());
  EXPECT_TRUE(utils::FindItemsUp<mvvm::test::toyitems::LayerItem>({multilayer}).empty());

  EXPECT_EQ(utils::FindItemsUp<mvvm::test::toyitems::LayerItem>({layer2, layer1, multilayer}),
            std::vector<mvvm::test::toyitems::LayerItem*>({layer2, layer1}));

  EXPECT_EQ(utils::FindItemsUp<mvvm::test::toyitems::LayerItem>({layer2, particle0}),
            std::vector<mvvm::test::toyitems::LayerItem*>({layer2, layer0}));
  EXPECT_EQ(
      utils::FindItemsUp<mvvm::test::toyitems::LayerItem>({layer2, particle0, layer1, layer0}),
      std::vector<mvvm::test::toyitems::LayerItem*>({layer2, layer0, layer1}));
}
