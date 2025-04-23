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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/model_composer.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/test/mock_model.h>

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Testing ModelComposer class.

class ModelComposerTests : public ::testing::Test
{
};

TEST_F(ModelComposerTests, InitialState)
{
  mvvm::test::MockModel model;
  ModelComposer composer(model);

  EXPECT_EQ(composer.GetModel(), &model);
}

TEST_F(ModelComposerTests, InsertItem)
{
  mvvm::test::MockModel model;
  ModelComposer composer(model);

  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  // inserting child
  auto inserted = composer.InsertItem(std::move(child), parent.get(), TagIndex());

  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem(TagIndex()), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
}

TEST_F(ModelComposerTests, TakeItem)
{
  mvvm::test::MockModel model;
  ModelComposer composer(model);

  // preparing parent
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(TagIndex::Append());

  EXPECT_EQ(parent->GetTotalItemCount(), 1);

  // taking item via composer
  auto taken = composer.TakeItem(parent.get(), TagIndex());

  EXPECT_EQ(taken->GetParent(), nullptr);
  EXPECT_EQ(taken.get(), child);
}

TEST_F(ModelComposerTests, SetData)
{
  mvvm::test::MockModel model;
  ModelComposer composer(model);

  SessionItem item;
  EXPECT_TRUE(composer.SetData(&item, 42, DataRole::kData));
  EXPECT_EQ(item.Data(), variant_t(42));

  // setting second time the same data is not possible
  EXPECT_FALSE(composer.SetData(&item, 42, DataRole::kData));
}

TEST_F(ModelComposerTests, Reset)
{
  mvvm::test::MockModel model;
  ModelComposer composer(model);

  auto parent0 = std::make_unique<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child0 = parent0->InsertItem<PropertyItem>(TagIndex::Append());

  auto new_root = utils::CreateEmptyRootItem();
  auto new_root_ptr = new_root.get();

  EXPECT_CALL(model, CheckIn(new_root_ptr)).Times(1);

  composer.ReplaceRootItem(parent0, std::move(new_root));

  EXPECT_EQ(parent0->GetTotalItemCount(), 0);
  EXPECT_EQ(parent0->GetModel(), &model);

  EXPECT_CALL(model, CheckOut(new_root_ptr)).Times(1);
  parent0.reset();
}
