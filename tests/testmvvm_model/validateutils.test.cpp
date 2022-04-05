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

#include "mvvm/model/validateutils.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taggeditems.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

using namespace mvvm;

class ValidateUtilsTest : public ::testing::Test
{
public:
  SessionModel m_model;
};

//! Check throw in ValidateItemMove when items are not defined, or do not have model/parent
//! assigned.

TEST_F(ValidateUtilsTest, ValidateItemMoveInvalidItems)
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

TEST_F(ValidateUtilsTest, ValidateItemMoveFromOneParentToAnother)
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

TEST_F(ValidateUtilsTest, ValidateItemMoveFromPropertyTag)
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

TEST_F(ValidateUtilsTest, ValidateItemMoveToPropertyTag)
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
