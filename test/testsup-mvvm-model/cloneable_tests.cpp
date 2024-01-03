/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
  {
    experimental::MyItem base;
    std::unique_ptr<experimental::MyItem> clone = base.Clone();

    // not compiling
    // std::unique_ptr<experimental::MyDerivedItem> derived_clone2 = base.Clone();
  }

  {
    experimental::MyDerivedItem derived;
    std::unique_ptr<experimental::MyDerivedItem> derived_clone = derived.Clone();
    EXPECT_TRUE(dynamic_cast<experimental::MyDerivedItem *>(derived_clone.get()));

    std::unique_ptr<experimental::MyItem> derived_clone2 = derived.Clone();
    EXPECT_TRUE(dynamic_cast<experimental::MyDerivedItem *>(derived_clone2.get()));
  }

  {
    experimental::MyDerivedItem derived;
    experimental::MyItem *base = &derived;

    auto derived_clone = derived.Clone();
    auto base_clone = base->Clone();

    EXPECT_TRUE(dynamic_cast<experimental::MyDerivedItem *>(derived_clone.get()));
    EXPECT_TRUE(dynamic_cast<experimental::MyDerivedItem *>(base_clone.get()));
  }
}
