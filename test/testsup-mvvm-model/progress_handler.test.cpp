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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/progress_handler.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ProgressHandlerTest : public ::testing::Test
{
};

TEST_F(ProgressHandlerTest, initialState)
{
  ProgressHandler handler;
  EXPECT_FALSE(handler.HasInterruptRequest());
}

TEST_F(ProgressHandlerTest, fullConstructor)
{
  size_t max_ticks = 1000;
  int progress{0};
  auto on_progress_change = [&progress](int value)
  {
    progress = value;
    return false;
  };

  ProgressHandler handler(on_progress_change, max_ticks);

  handler.SetCompletedTicks(100);
  EXPECT_FALSE(handler.HasInterruptRequest());
  EXPECT_EQ(progress, 10);

  handler.SetCompletedTicks(900);
  EXPECT_FALSE(handler.HasInterruptRequest());
  EXPECT_EQ(progress, 100);  // reports value in percents
}

TEST_F(ProgressHandlerTest, interruptRequest)
{
  size_t max_ticks = 1000;
  int progress{0};
  auto on_progress_change = [&progress](int value)
  {
    progress = value;
    return true;
  };

  ProgressHandler handler(on_progress_change, max_ticks);

  handler.SetCompletedTicks(1000);
  EXPECT_TRUE(handler.HasInterruptRequest());
  EXPECT_EQ(progress, 100);  // reports value in percents

  // checking reset
  handler.Reset();
  EXPECT_FALSE(handler.HasInterruptRequest());
  handler.SetCompletedTicks(100);
  EXPECT_EQ(progress, 10);  // reports value in percents
}
