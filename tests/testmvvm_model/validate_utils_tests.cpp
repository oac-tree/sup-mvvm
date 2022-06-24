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

#include "mvvm/model/validate_utils.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/tagged_items.h>

#include <memory>
#include <stdexcept>

using namespace mvvm;

class ValidateUtilsTests : public ::testing::Test
{
public:
  SessionModel m_model;
};

TEST_F(ValidateUtilsTests, GetActualInsertTagIndex)
{
  using ::mvvm::utils::GetActualInsertTagIndex;

  CompoundItem item;
  EXPECT_THROW(GetActualInsertTagIndex(&item, TagIndex::Append()), InvalidInsertException);

  // registering default tag
  item.RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  // checking that uninitialised tag is correctly converted to the right tag
  EXPECT_EQ(GetActualInsertTagIndex(&item, {"", -1}), TagIndex("tag", 0));

  // inserting an item, checking if tag points to the next one after
  item.InsertItem<SessionItem>({"tag", 0});
  EXPECT_EQ(GetActualInsertTagIndex(&item, {"", -1}), TagIndex("tag", 1));

  // Wrong tag will be returned as it is. It will be validated in ValidateItemInsert method.
  EXPECT_EQ(GetActualInsertTagIndex(&item, {"abc", 0}), TagIndex("abc", 0));
}

//! Check throw in ValidateItemInsert when items are not defined, or do not have model/parent
//! assigned.

TEST_F(ValidateUtilsTests, ValidateItemInsertInvalidItems)
{
  using ::mvvm::utils::ValidateItemInsert;

  // invalid items
  EXPECT_THROW(ValidateItemInsert(nullptr, nullptr, TagIndex()), InvalidInsertException);

  // item without model
  SessionItem item;
  CompoundItem parent;
  EXPECT_THROW(ValidateItemInsert(&item, &parent, TagIndex()), InvalidInsertException);
}

//! Check throw in ValidateItemInsert when item is inserted to itself.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenInsertToItself)
{
  using ::mvvm::utils::ValidateItemInsert;

  auto item = std::make_unique<CompoundItem>();
  item->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  EXPECT_THROW(ValidateItemInsert(item.get(), item.get(), {"tag1", 0}), InvalidInsertException);
}

//! Check throw in ValidateItemInsert when item belongs already to another item.

TEST_F(ValidateUtilsTests, ValidateItemInsertOnAttemptToMakeParentAChild)
{
  using ::mvvm::utils::ValidateItemInsert;

  auto child = m_model.InsertItem<CompoundItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  EXPECT_THROW(ValidateItemInsert(m_model.GetRootItem(), child, {"tag1", 0}),
               InvalidInsertException);
}

//! Check throw in ValidateItemInsert when item belongs already to another item.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenItemBelongsToAnotherParent)
{
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  EXPECT_THROW(ValidateItemInsert(child, parent0, {"tag1", 0}), InvalidInsertException);
}

//! Check no_throw in ValidateItemInsert when insertion is valid.

TEST_F(ValidateUtilsTests, ValidateItemInsertInDefaultTag)
{
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  CompoundItem candidate;

  EXPECT_NO_THROW(ValidateItemInsert(&candidate, parent0, {"tag1", 0}));
  EXPECT_NO_THROW(ValidateItemInsert(&candidate, parent0, {"", -1}));
}

//! Check throw in ValidateItemInsert when no default tag is present.

TEST_F(ValidateUtilsTests, ValidateItemInsertWhenNoDefaultTagIsPresent)
{
  using ::mvvm::utils::ValidateItemInsert;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ false);

  CompoundItem candidate;

  EXPECT_THROW(ValidateItemInsert(&candidate, parent0, {"", -1}), InvalidInsertException);
}

//! Check throw in ValidateItemMove when items are not defined, or do not have model/parent
//! assigned.

TEST_F(ValidateUtilsTests, ValidateItemMoveInvalidItems)
{
  using ::mvvm::utils::ValidateItemMove;

  // invalid items
  EXPECT_THROW(ValidateItemMove(nullptr, nullptr, TagIndex()), InvalidMoveException);

  // item without model
  SessionItem item;
  CompoundItem parent;
  EXPECT_THROW(ValidateItemMove(&item, &parent, TagIndex()), InvalidMoveException);
}

//! Check ValidateItemMove when item is property moved from one parent to another.

TEST_F(ValidateUtilsTests, ValidateItemMoveOnAttemptToMoveParentToChild)
{
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // attempt to move parent into parent
  EXPECT_THROW(ValidateItemMove(parent0, parent0, {"tag1", 0}), InvalidMoveException);

  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // attempt to move parent to child
  EXPECT_THROW(ValidateItemMove(parent0, child, {"tag1", 0}), InvalidMoveException);
}

//! Check ValidateItemMove when item is property moved from one parent to another.

TEST_F(ValidateUtilsTests, ValidateItemMoveFromOneParentToAnother)
{
  using ::mvvm::utils::ValidateItemMove;

  // invalid items
  EXPECT_THROW(ValidateItemMove(nullptr, nullptr, TagIndex()), InvalidMoveException);

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // item without model
  EXPECT_NO_THROW(ValidateItemMove(child, parent1, {"tag1", 0}));
}

//! Check ValidateItemMove when trying to move property item to another parent.

TEST_F(ValidateUtilsTests, ValidateItemMoveFromPropertyTag)
{
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  auto property = parent0->AddProperty("thickness", 42);

  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // it is not allowed to take property item
  EXPECT_THROW(ValidateItemMove(property, parent1, {"tag1", 0}), InvalidMoveException);
}

//! Check ValidateItemMove when trying to move item into property tag of another item.

TEST_F(ValidateUtilsTests, ValidateItemMoveToPropertyTag)
{
  using ::mvvm::utils::ValidateItemMove;

  auto parent0 = m_model.InsertItem<CompoundItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<CompoundItem>(parent0, {"tag1", 0});

  auto parent1 = m_model.InsertItem<CompoundItem>();
  auto property = parent1->AddProperty("thickness", 42);

  // invalid move of property item
  EXPECT_THROW(ValidateItemMove(child, parent1, {"thickness", 0}), InvalidMoveException);
}
