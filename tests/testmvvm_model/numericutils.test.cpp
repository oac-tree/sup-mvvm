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

#include "mvvm/utils/numericutils.h"

#include <gtest/gtest.h>

#include <cmath>

using namespace mvvm;

class NumericUtilsTest : public ::testing::Test
{
};

TEST_F(NumericUtilsTest, areAlmostEqual)
{
  EXPECT_TRUE(utils::AreAlmostEqual(0.0, 0.0));
  EXPECT_TRUE(utils::AreAlmostEqual(1.0, 1.0));
  EXPECT_TRUE(utils::AreAlmostEqual(10.0 / 100.0, 100.0 / 1000.0));
  EXPECT_TRUE(utils::AreAlmostEqual(std::sin(0.0), 0.0));
  EXPECT_FALSE(utils::AreAlmostEqual(std::cos(0.0), 0.0));
}
