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

#include "mvvm/utils/threadsafe_queue.h"

#include <gtest/gtest.h>

#include <future>

using namespace mvvm;

//! Testing threadsafe_queue.

class ThreadSafeQueueTests : public ::testing::Test
{
};

//! Checking stack initial state (single thread mode).

TEST_F(ThreadSafeQueueTests, InitialState)
{
  threadsafe_queue<int> queue;
  EXPECT_TRUE(queue.empty());
  int value;
  EXPECT_FALSE(queue.try_pop(value));

  auto sh_value = queue.try_pop();
  EXPECT_FALSE(sh_value);
}

//! Push and then pop (single thread mode).

TEST_F(ThreadSafeQueueTests, PushAndPop)
{
  threadsafe_queue<int> queue;

  queue.push(42);
  EXPECT_FALSE(queue.empty());
  int value(0);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 42);

  queue.push(43);
  auto result = queue.wait_and_pop();
  EXPECT_EQ(*result.get(), 43);
}

//! Multiple push and pops (single thread mode).

TEST_F(ThreadSafeQueueTests, MultiplePushAndPop)
{
  threadsafe_queue<int> queue;

  queue.push(42);
  queue.push(43);
  queue.push(44);
  EXPECT_FALSE(queue.empty());
  int value(0);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 43);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 44);

  auto result = queue.try_pop();
  EXPECT_EQ(result.get(), nullptr);
}
