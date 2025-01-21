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

#include "mvvm/model/validate_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/tagged_items.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

class ValidateUtilsTests : public ::testing::Test
{
public:
  SessionModel m_model;
};

TEST_F(ValidateUtilsTests, GetInsertTagIndex)
{
  using ::mvvm::utils::GetInsertTagIndex;

  CompoundItem item;
  EXPECT_FALSE(GetInsertTagIndex(&item, TagIndex::Append()).IsValid());

  // registering default tag
  item.RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  // checking that uninitialised tag is correctly converted to the right tag
  EXPECT_EQ(GetInsertTagIndex(&item, TagIndex::Append()), TagIndex("tag", 0));

  // inserting an item, checking if tag points to the next one after
  item.InsertItem<SessionItem>({"tag", 0});
  EXPECT_EQ(GetInsertTagIndex(&item, TagIndex::Append()), TagIndex("tag", 1));

  // Wrong tag
  EXPECT_FALSE(GetInsertTagIndex(&item, {"abc", 0}).IsValid());
}

//! Check throw in ValidateItemInsert when items are not defined, or do not have model/parent
//! assigned.

TEST_F(ValidateUtilsTests, ValidateItemInsertInvalidItems)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  // invalid items
  EXPECT_FALSE(CanInsertItem(nullptr, nullptr, TagIndex()).first);
  EXPECT_THROW(ValidateItemInsert(nullptr, nullptr, TagIndex()), InvalidOperationException);

  // item without model
  SessionItem item;
  CompoundItem parent;
  EXPECT_FALSE(CanInsertItem(&item, &parent, TagIndex()).first);
  EXPECT_THROW(ValidateItemInsert(&item, &parent, TagIndex()), InvalidOperationException);
}

//! Check throw in ValidateItemInsert when item is inserted to itself.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenInsertToItself)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  auto item = std::make_unique<CompoundItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  EXPECT_FALSE(CanInsertItem(item.get(), item.get(), {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemInsert(item.get(), item.get(), {"tag1", 0}), InvalidOperationException);
}

//! Check throw in ValidateItemInsert when item belongs already to another item.

TEST_F(ValidateUtilsTests, ValidateItemInsertOnAttemptToMakeParentAChild)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  auto child = m_model.InsertItem<CompoundItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  EXPECT_FALSE(CanInsertItem(m_model.GetRootItem(), child, {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemInsert(m_model.GetRootItem(), child, {"tag1", 0}),
               InvalidOperationException);
}

//! Check throw in ValidateItemInsert when item belongs already to another item.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenItemBelongsToAnotherParent)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  EXPECT_FALSE(CanInsertItem(child, parent0, {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemInsert(child, parent0, {"tag1", 0}), InvalidOperationException);
}

//! Check no_throw in ValidateItemInsert when insertion is valid.

TEST_F(ValidateUtilsTests, ValidateItemInsertInDefaultTag)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  CompoundItem candidate;

  EXPECT_TRUE(CanInsertItem(&candidate, parent0, {"tag1", 0}).first);
  EXPECT_NO_THROW(ValidateItemInsert(&candidate, parent0, {"tag1", 0}));

  // we do allow  implicit conversion of TagIndex::Append into real index
  EXPECT_TRUE(CanInsertItem(&candidate, parent0, TagIndex::Append()).first);
  EXPECT_NO_THROW(ValidateItemInsert(&candidate, parent0, TagIndex::Append()));
}

//! Check throw in ValidateItemInsert when no default tag is present.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenNoDefaultTagIsPresent)
{
  using ::mvvm::utils::CanInsertItem;
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ false);

  CompoundItem candidate;

  EXPECT_FALSE(CanInsertItem(&candidate, parent0, TagIndex::Append()).first);
  EXPECT_THROW(ValidateItemInsert(&candidate, parent0, TagIndex::Append()),
               InvalidOperationException);
}

//! Check  ValidateItemTypeInsert when item belongs already to another item.

TEST_F(ValidateUtilsTests, ValidateItemTypeInsert)
{
  using ::mvvm::utils::CanInsertType;
  using ::mvvm::utils::ValidateItemTypeInsert;

  const std::string item_type("some-item-type");

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1", {item_type}), /*set_as_default*/ true);

  // inserting a child of correct type
  auto child = std::make_unique<CompoundItem>(item_type);
  m_model.InsertItem(std::move(child), parent0, {"tag1", 0});

  EXPECT_TRUE(CanInsertType(item_type, parent0, {"tag1", 0}).first);
  EXPECT_TRUE(CanInsertType(item_type, parent0, {"tag1", 1}).first);
  EXPECT_FALSE(CanInsertType(item_type, parent0, {"tag1", 2}).first);               // wrong index
  EXPECT_FALSE(CanInsertType(item_type, parent0, {"non-existing-type", 1}).first);  // wrong tag

  // we do allow  implicit conversion of TagIndex::Append into real index
  EXPECT_TRUE(CanInsertType(item_type, parent0, TagIndex::Append()).first);

  // not-registered item type
  EXPECT_FALSE(CanInsertType("wrong-type", parent0, {"tag1", 0}).first);  // wrong index
  EXPECT_THROW(ValidateItemTypeInsert("wrong-type", parent0, {"tag1", 0}),
               InvalidOperationException);
}

//! Check throw in ValidateItemMove when items are not defined, or do not have model/parent
//! assigned.

TEST_F(ValidateUtilsTests, ValidateItemMoveInvalidItems)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  // invalid items
  EXPECT_FALSE(CanMoveItem(nullptr, nullptr, TagIndex()).first);
  EXPECT_THROW(ValidateItemMove(nullptr, nullptr, TagIndex()), InvalidOperationException);

  // item without model
  SessionItem item;
  CompoundItem parent;
  EXPECT_FALSE(CanMoveItem(&item, &parent, TagIndex()).first);
  EXPECT_THROW(ValidateItemMove(&item, &parent, TagIndex()), InvalidOperationException);
}

//! Check ValidateItemMove when item is property moved from one parent to another.

TEST_F(ValidateUtilsTests, ValidateItemMoveOnAttemptToMoveParentToChild)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // attempt to move parent into parent
  EXPECT_FALSE(CanMoveItem(parent0, parent0, {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemMove(parent0, parent0, {"tag1", 0}), InvalidOperationException);

  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // attempt to move parent to child
  EXPECT_FALSE(CanMoveItem(parent0, child, {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemMove(parent0, child, {"tag1", 0}), InvalidOperationException);
}

//! Check ValidateItemMove when item is property moved from one parent to another.

TEST_F(ValidateUtilsTests, ValidateItemMoveFromOneParentToAnother)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  // invalid items
  EXPECT_FALSE(CanMoveItem(nullptr, nullptr, TagIndex()).first);
  EXPECT_THROW(ValidateItemMove(nullptr, nullptr, TagIndex()), InvalidOperationException);

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  EXPECT_TRUE(CanMoveItem(child, parent1, {"tag1", 0}).first);
  EXPECT_NO_THROW(ValidateItemMove(child, parent1, {"tag1", 0}));
}

//! Check ValidateItemMove when trying to move property item to another parent.

TEST_F(ValidateUtilsTests, ValidateItemMoveFromPropertyTag)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  auto& property = parent0->AddProperty("thickness", 42);

  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // it is not allowed to take property item
  EXPECT_FALSE(CanMoveItem(&property, parent1, {"tag1", 0}).first);
  EXPECT_THROW(ValidateItemMove(&property, parent1, {"tag1", 0}), InvalidOperationException);
}

//! Check ValidateItemMove when trying to move item into property tag of another item.

TEST_F(ValidateUtilsTests, ValidateItemMoveToPropertyTag)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  auto parent1 = m_model.InsertItem<CompoundItem>();
  auto& property = parent1->AddProperty("thickness", 42);

  // invalid move of property item
  EXPECT_FALSE(CanMoveItem(child, parent1, {"thickness", 0}).first);
  EXPECT_THROW(ValidateItemMove(child, parent1, {"thickness", 0}), InvalidOperationException);
}

//! Check ValidateItemMove when trying to move item into property tag of another item.

TEST_F(ValidateUtilsTests, ValidateItemMoveToLast)
{
  using ::mvvm::utils::CanMoveItem;
  using ::mvvm::utils::ValidateItemMove;

  auto item0 = m_model.InsertItem<SessionItem>(m_model.GetRootItem());  // 0
  auto item1 = m_model.InsertItem<SessionItem>(m_model.GetRootItem());  // 1
  auto item2 = m_model.InsertItem<SessionItem>(m_model.GetRootItem());  // 2
  auto item3 = m_model.InsertItem<SessionItem>(m_model.GetRootItem());  // 3

  EXPECT_TRUE(CanMoveItem(item0, m_model.GetRootItem(), {"", 3}).first);
  EXPECT_NO_THROW(ValidateItemMove(item0, m_model.GetRootItem(), {"", 3}));

  // invalid move of item too far back
  EXPECT_FALSE(CanMoveItem(item0, m_model.GetRootItem(), {"", 4}).first);
  EXPECT_THROW(ValidateItemMove(item0, m_model.GetRootItem(), {"", 4}), InvalidOperationException);
}

//! Check ValidateTakeItem when parent is not defined.

TEST_F(ValidateUtilsTests, ValidateTakeItemWhenParentIsNotDefined)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;
  SessionItem item;

  EXPECT_FALSE(CanTakeItem(nullptr, {}).first);
  EXPECT_THROW(ValidateTakeItem(nullptr, {}), InvalidOperationException);
}

//! Check ValidateTakeItem when model is not defined.

TEST_F(ValidateUtilsTests, ValidateTakeItemWhenModelIsNotDefined)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;
  auto parent = m_model.InsertItem<CompoundItem>();

  EXPECT_FALSE(CanTakeItem(parent, {}).first);
  EXPECT_THROW(ValidateTakeItem(parent, {}), InvalidOperationException);
}

//! Check ValidateTakeItem when parent doesn't belong to a model.

TEST_F(ValidateUtilsTests, ValidateTakeItemWhenParentDoesntBelongToModel)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;

  SessionItem parent;

  EXPECT_FALSE(CanTakeItem(&parent, {}).first);
  EXPECT_THROW(ValidateTakeItem(&parent, {}), InvalidOperationException);
}

//! Check ValidateTakeItem on attempt to remove property.

TEST_F(ValidateUtilsTests, ValidateTakeItemFromPropertyTag)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;

  auto parent = m_model.InsertItem<CompoundItem>();
  auto& property = parent->AddProperty("thickness", 42);

  EXPECT_FALSE(CanTakeItem(parent, {"thickness", 0}).first);
  EXPECT_THROW(ValidateTakeItem(parent, {"thickness", 0}), InvalidOperationException);
}

//! Check ValidateTakeItem when parent doesn't belong to a model.

TEST_F(ValidateUtilsTests, ValidateTakeItemFromNonExistingIndex)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;

  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent, {"tag1", 0});

  EXPECT_FALSE(CanTakeItem(parent, {"tag1", 1}).first);
  EXPECT_THROW(ValidateTakeItem(parent, {"tag1", 1}), InvalidOperationException);
}

//! Check ValidateTakeItem on valid request.

TEST_F(ValidateUtilsTests, ValidateTakeItemOnValidRequest)
{
  using ::mvvm::utils::CanTakeItem;
  using ::mvvm::utils::ValidateTakeItem;

  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent, {"tag1", 0});

  EXPECT_TRUE(CanTakeItem(parent, {"tag1", 0}).first);
  EXPECT_NO_THROW(ValidateTakeItem(parent, {"tag1", 0}));
}
