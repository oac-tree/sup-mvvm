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

#include "mvvm/standarditems/container_item.h"

#include <gtest/gtest.h>

#include <mvvm/standarditems/vector_item.h>

using namespace mvvm;
#include <iostream>

//! Test of ContainerItem.

class ContainerItemTests : public ::testing::Test
{
};

TEST_F(ContainerItemTests, InitialState)
{
  ContainerItem item;
  EXPECT_TRUE(item.IsEmpty());
}

TEST_F(ContainerItemTests, IsEmpty)
{
  ContainerItem item;
  item.InsertItem<PropertyItem>({"", 0});
  EXPECT_FALSE(item.IsEmpty());
}

TEST_F(ContainerItemTests, GetSize)
{
  ContainerItem item;
  EXPECT_EQ(item.GetSize(), 0);
  item.InsertItem<PropertyItem>({"", 0});
  EXPECT_FALSE(item.IsEmpty());
  EXPECT_EQ(item.GetSize(), 1);
}

TEST_F(ContainerItemTests, Clone)
{
  // creating container with two elements
  ContainerItem item;
  auto property = item.InsertItem<PropertyItem>(TagIndex::Append());
  property->SetData(42);
  property->SetToolTip("abc");
  auto vector = item.InsertItem<VectorItem>(TagIndex::Append());
  vector->SetXYZ(1.0, 2.0, 3.0);

  // making container clone
  auto clone = item.Clone(/* make_unique_id*/ false);
  auto cloned_container = dynamic_cast<ContainerItem*>(clone.get());
  ASSERT_NE(cloned_container, nullptr);

  EXPECT_EQ(cloned_container->GetSize(), 2);

  // validating first element
  auto property_clone = cloned_container->GetItem<PropertyItem>({"", 0});
  ASSERT_NE(property_clone, nullptr);
  EXPECT_EQ(property_clone->GetToolTip(), std::string("abc"));
  EXPECT_EQ(property_clone->Data<int>(), 42);
  EXPECT_EQ(property_clone->GetParent(), cloned_container);

  // validating vector
  auto vector_clone = cloned_container->GetItem<VectorItem>({"", 1});
  ASSERT_NE(vector_clone, nullptr);
  EXPECT_EQ(vector_clone->GetIdentifier(), vector->GetIdentifier());
  EXPECT_EQ(vector_clone->GetParent(), cloned_container);
  EXPECT_DOUBLE_EQ(vector_clone->X(), 1.0);
  EXPECT_DOUBLE_EQ(vector_clone->Y(), 2.0);
  EXPECT_DOUBLE_EQ(vector_clone->Z(), 3.0);

  // validating vector's children
  auto x_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kX));
  auto y_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kY));
  auto z_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kZ));
  ASSERT_NE(x_clone, nullptr);
  ASSERT_NE(y_clone, nullptr);
  ASSERT_NE(z_clone, nullptr);
  EXPECT_EQ(x_clone->GetParent(), vector_clone);
  EXPECT_EQ(y_clone->GetParent(), vector_clone);
  EXPECT_EQ(z_clone->GetParent(), vector_clone);
}
