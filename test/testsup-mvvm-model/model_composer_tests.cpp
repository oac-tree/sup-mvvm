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

#include "mvvm/model/model_composer.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>
#include <testutils/mock_model.h>

using namespace mvvm;
using ::testing::_;

//! Testing ModelComposer class.

class ModelComposerTests : public ::testing::Test
{
};

TEST_F(ModelComposerTests, InitialState)
{
  testutils::MockModel model;
  ModelComposer composer(model);

  EXPECT_EQ(composer.GetModel(), &model);
}

TEST_F(ModelComposerTests, InsertItem)
{
  testutils::MockModel model;
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
  testutils::MockModel model;
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
  testutils::MockModel model;
  ModelComposer composer(model);

  SessionItem item;
  EXPECT_TRUE(composer.SetData(&item, 42, DataRole::kData));
  EXPECT_EQ(item.Data(), variant_t(42));

  // setting second time the same data is not possible
  EXPECT_FALSE(composer.SetData(&item, 42, DataRole::kData));
}

TEST_F(ModelComposerTests, Reset)
{
  testutils::MockModel model;
  ModelComposer composer(model);

  auto parent0 = std::make_unique<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child0 = parent0->InsertItem<PropertyItem>(TagIndex::Append());

  composer.Reset(parent0, {});
  EXPECT_EQ(parent0->GetTotalItemCount(), 0);
  EXPECT_EQ(parent0->GetModel(), &model);
}
