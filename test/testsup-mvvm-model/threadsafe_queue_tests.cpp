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

#include "mvvm/utils/threadsafe_queue.h"

#include <gtest/gtest.h>

#include <algorithm>
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
  int value{0};
  EXPECT_FALSE(queue.try_pop(value));
  EXPECT_EQ(queue.size(), 0);

  auto sh_value = queue.try_pop();
  EXPECT_FALSE(sh_value);
}

//! Push and then pop (single thread mode).

TEST_F(ThreadSafeQueueTests, PushAndPop)
{
  threadsafe_queue<int> queue;

  queue.push(42);
  EXPECT_EQ(queue.size(), 1);
  EXPECT_FALSE(queue.empty());
  int value(0);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_EQ(queue.size(), 0);

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
  EXPECT_EQ(queue.size(), 3);
  EXPECT_FALSE(queue.empty());
  int value(0);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 43);
  EXPECT_TRUE(queue.try_pop(value));
  EXPECT_EQ(value, 44);
  EXPECT_EQ(queue.size(), 0);

  auto result = queue.try_pop();
  EXPECT_EQ(result.get(), nullptr);
}

//! Push and pop in concurrent mode. Test is based on ideas from Anthony Williams, C++ Concurrency
//! in Action, Second edition. Two threads are filling concurrently the queue with unique numbers.
//! Simultaneously, the reading thread reads the values from the queue. We expect all number read
//! out without duplications.

TEST_F(ThreadSafeQueueTests, ConcurentPushAndPop)
{
  threadsafe_queue<int> queue;
  std::promise<void> go;
  std::shared_future<void> ready(go.get_future());

  try
  {
    // starting thread1 for pushing
    std::promise<void> push1_ready_for_test;
    auto push_action1 = [&queue, ready, &push1_ready_for_test]()
    {
      push1_ready_for_test.set_value();  // reporting that we are ready to go
      ready.wait();                      // waiting for command to start the race

      for (int i = 0; i < 10; ++i)
      {
        queue.push(i);
      }
    };
    std::future<void> push_done1 = std::async(std::launch::async, push_action1);

    // starting thread2 for pushing
    std::promise<void> push2_ready_for_test;
    auto push_action2 = [&queue, ready, &push2_ready_for_test]()
    {
      push2_ready_for_test.set_value();  // reporting that we are ready to go
      ready.wait();                      // waiting for command to start the race

      for (int i = 10; i < 20; ++i)
      {
        queue.push(i);
      }
    };
    std::future<void> push_done2 = std::async(std::launch::async, push_action2);

    // starting pop thread
    std::promise<void> pop_ready_for_test;
    auto pop_action = [&queue, ready, &pop_ready_for_test]()
    {
      pop_ready_for_test.set_value();  // reporting that we are ready to go
      ready.wait();                    // waiting for command to start the race

      std::vector<int> result;
      for (int i = 0; i < 20; ++i)
      {
        int index{0};
        queue.wait_and_pop(index);
        result.push_back(index);
      }

      return result;
    };
    std::future<std::vector<int>> pop_done = std::async(std::launch::async, pop_action);

    // waiting for threads being prepared for racing
    push1_ready_for_test.get_future().wait();
    push2_ready_for_test.get_future().wait();
    pop_ready_for_test.get_future().wait();

    // starting concurrent push and pop
    go.set_value();

    // checking result
    push_done1.get();  // making sure pushing thread1 has finished
    push_done2.get();  // making sure pushing thread2 has finished

    std::vector<int> result = pop_done.get();
    EXPECT_EQ(result.size(), 20);
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(*std::min_element(result.begin(), result.end()), 0);
    EXPECT_EQ(*std::max_element(result.begin(), result.end()), 19);

    // checking there is no duplication
    const std::set<int> filtered_result(result.begin(), result.end());
    EXPECT_EQ(filtered_result.size(), 20);
  }
  catch (...)
  {
    go.set_value();
    throw;
  }
}
