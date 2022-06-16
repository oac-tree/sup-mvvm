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

#include "mvvm/factories/item_catalogue_factory.h"

#include <mvvm/standarditems/standard_item_includes.h>

#include <gtest/gtest.h>

using namespace mvvm;

class ItemCatalogueFactoryTest : public ::testing::Test
{
};

TEST_F(ItemCatalogueFactoryTest, CreateStandardItemCatalogue)
{
  auto catalogue = CreateStandardItemCatalogue();

  auto item = catalogue->Create(SessionItem::Type);
  EXPECT_TRUE(dynamic_cast<SessionItem*>(item.get()) != nullptr);

  item = catalogue->Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  item = catalogue->Create(VectorItem::Type);
  EXPECT_TRUE(dynamic_cast<VectorItem*>(item.get()) != nullptr);

  item = catalogue->Create(CompoundItem::Type);
  EXPECT_TRUE(dynamic_cast<CompoundItem*>(item.get()) != nullptr);
}

TEST_F(ItemCatalogueFactoryTest, AddStandardItemsToCatalogue)
{
  ItemCatalogue<SessionItem> catalogue;
  AddStandardItemsToCatalogue(catalogue);

  auto item = catalogue.Create(SessionItem::Type);
  EXPECT_TRUE(dynamic_cast<SessionItem*>(item.get()) != nullptr);

  item = catalogue.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  item = catalogue.Create(VectorItem::Type);
  EXPECT_TRUE(dynamic_cast<VectorItem*>(item.get()) != nullptr);

  item = catalogue.Create(CompoundItem::Type);
  EXPECT_TRUE(dynamic_cast<CompoundItem*>(item.get()) != nullptr);
}
