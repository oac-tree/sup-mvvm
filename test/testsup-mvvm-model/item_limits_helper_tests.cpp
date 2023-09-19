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

#include "mvvm/model/item_limits_helper.h"
#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Unit tests for helper functions defined in item_limits_helper.h

class ItemLimitsHelperTests : public ::testing::Test
{
};

TEST_F(ItemLimitsHelperTests, LowerLimited)
{
  SessionItem item;
  EXPECT_FALSE(HasLowerLimit(item));
}
