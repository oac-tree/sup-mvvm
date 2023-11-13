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

#include "mvvm/model/property_item.h"

#include <mvvm/model/limits.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing PropertyItem class.

class PropertyItemTests : public ::testing::Test
{
};

TEST_F(PropertyItemTests, SetLimits)
{
  PropertyItem item;

  EXPECT_FALSE(item.HasData(DataRole::kLimits));
  item.SetData(42);
}

TEST_F(PropertyItemTests, Clone)
{
  PropertyItem item;
  item.SetData(42);
  item.SetDisplayName("abc");

  {  // deep copy
    auto clone = item.Clone(/* make_unique_id*/ true);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_NE(clone->GetIdentifier(), item.GetIdentifier());
    EXPECT_EQ(clone->GetDisplayName(), item.GetDisplayName());
    EXPECT_EQ(clone->Data<int>(), 42);
  }

  {  // clone
    auto clone = item.Clone(/* make_unique_id*/ false);
    EXPECT_NE(dynamic_cast<PropertyItem*>(clone.get()), nullptr);
    EXPECT_EQ(clone->GetIdentifier(), item.GetIdentifier());
    EXPECT_EQ(clone->GetDisplayName(), item.GetDisplayName());
    EXPECT_EQ(clone->Data<int>(), 42);
  }
}
