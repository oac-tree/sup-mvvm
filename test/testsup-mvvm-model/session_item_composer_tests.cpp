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

#include "mvvm/serialization/session_item_composer.h"

#include <mvvm/model/item_factory.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_composer.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for SessionItemComposer class.
 */
class SessionItemComposerTest : public ::testing::Test
{
public:
  static std::unique_ptr<tree_data_t> CreateTreeData(const SessionItem& item)
  {
    TreeDataComposer composer({});
    return composer.CreateTreeData(item);
  }

  static std::unique_ptr<SessionItem> CreateItem(const tree_data_t& tree_data, ConverterMode mode)
  {
    SessionItemComposer composer({&GetGlobalItemFactory(), mode});
    return composer.CreateSessionItem(tree_data);
  }
};

TEST_F(SessionItemComposerTest, FromSessionItem)
{
  SessionItem item;
  item.SetData(mvvm::int32{42});

  const auto tree_data = CreateTreeData(item);

  {
    auto reco_item = CreateItem(*tree_data, ConverterMode::kClone);
    EXPECT_EQ(reco_item->GetIdentifier(), item.GetIdentifier());
    EXPECT_EQ(reco_item->Data<mvvm::int32>(), mvvm::int32{42});
  }

  {
    auto reco_item = CreateItem(*tree_data, ConverterMode::kCopy);
    EXPECT_NE(reco_item->GetIdentifier(), item.GetIdentifier());
    EXPECT_EQ(reco_item->Data<mvvm::int32>(), mvvm::int32{42});
  }
}

TEST_F(SessionItemComposerTest, FromCompountItemWithProperty)
{
  CompoundItem item;
  auto property = item.AddProperty("width", mvvm::int32{42});

  const auto tree_data = CreateTreeData(item);

  {
    auto reco_item = CreateItem(*tree_data, ConverterMode::kClone);
    EXPECT_EQ(reco_item->GetIdentifier(), item.GetIdentifier());
    auto reco_property = reco_item->GetItem("width");
    EXPECT_EQ(reco_property->GetIdentifier(), property.GetIdentifier());
    EXPECT_EQ(reco_property->Data<mvvm::int32>(), mvvm::int32{42});
  }

  {
    auto reco_item = CreateItem(*tree_data, ConverterMode::kCopy);
    EXPECT_NE(reco_item->GetIdentifier(), item.GetIdentifier());
    auto reco_property = reco_item->GetItem("width");
    EXPECT_NE(reco_property->GetIdentifier(), property.GetIdentifier());
    EXPECT_EQ(reco_property->Data<mvvm::int32>(), mvvm::int32{42});
  }
}

}  // namespace mvvm::test
