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

#include "mvvm/experimental/cloneable.h"

#include <gtest/gtest.h>

class CloneableTests : public ::testing::Test
{
};

TEST_F(CloneableTests, InitialState)
{
  experimental::MyItem item;
  std::unique_ptr<experimental::MyItem> clone = item.Clone();

  experimental::MyDerivedItem derived_item;

  std::unique_ptr<experimental::MyDerivedItem> derived_clone = derived_item.Clone();

  std::unique_ptr<experimental::MyItem> clone2 = derived_item.Clone();
  EXPECT_TRUE(dynamic_cast<experimental::MyDerivedItem*>(clone2.get()));

  //  experimental::concrete cc;
}
