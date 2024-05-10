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

#include "mvvm/utils/threadsafe_stack.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <future>
#include <vector>

using namespace mvvm;

//! Testing threadsafe_stack.

class ThreadSafeStackTests : public ::testing::Test
{
};

//! Checking stack initial state (single thread mode).

TEST_F(ThreadSafeStackTests, InitialState)
{
  threadsafe_stack<int> stack;
  EXPECT_TRUE(stack.empty());
  int value;
  EXPECT_FALSE(stack.try_pop(value));
  EXPECT_EQ(stack.size(), 0);

  auto sh_value = stack.try_pop();
  EXPECT_FALSE(sh_value);
}

//! Push and then pop (single thread mode).

TEST_F(ThreadSafeStackTests, PushAndPop)
{
  threadsafe_stack<int> stack;

  stack.push(42);
  EXPECT_EQ(stack.size(), 1);
  EXPECT_FALSE(stack.empty());
  int value(0);
  EXPECT_TRUE(stack.try_pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_EQ(stack.size(), 0);

  stack.push(43);
  auto result = stack.wait_and_pop();
  EXPECT_EQ(*result.get(), 43);
}

//! Push and then pop (single thread mode).

TEST_F(ThreadSafeStackTests, MultiplePushAndPop)
{
  threadsafe_stack<int> stack;

  stack.push(42);
  stack.push(43);
  stack.push(44);
  EXPECT_EQ(stack.size(), 3);
  EXPECT_FALSE(stack.empty());
  int value(0);
  EXPECT_TRUE(stack.try_pop(value));
  EXPECT_EQ(value, 44);
  EXPECT_TRUE(stack.try_pop(value));
  EXPECT_EQ(value, 43);
  EXPECT_TRUE(stack.try_pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_EQ(stack.size(), 0);

  auto result = stack.try_pop();
  EXPECT_EQ(result.get(), nullptr);
}

//! Update top value (single thread mode).

TEST_F(ThreadSafeStackTests, UpdateTop)
{
  threadsafe_stack<int> stack;

  // update of empty stack means simple appearance of value
  stack.update_top(42);
  EXPECT_FALSE(stack.empty());
  int value(0);
  EXPECT_TRUE(stack.try_pop(value));
  EXPECT_EQ(value, 42);

  // updating value
  stack.push(43);
  stack.update_top(44);
  auto result = stack.wait_and_pop();
  EXPECT_EQ(*result.get(), 44);

  // shouldn't be more values
  auto sh_value = stack.try_pop();
  EXPECT_FALSE(sh_value);
}

//! Push and pop in concurrent mode.
//! Test is borrowed from Anthony Williams, C++ Concurrency in Action, Second edition.

TEST_F(ThreadSafeStackTests, ConcurentPushAndPop)
{
  threadsafe_stack<int> stack;
  std::promise<void> go;
  std::shared_future<void> ready(go.get_future());

  try
  {
    std::promise<void> push_ready_for_test;
    auto push_action = [&stack, ready, &push_ready_for_test]()
    {
      push_ready_for_test.set_value();  // reporting that we are ready to go
      ready.wait();                     // waiting for command to start the race
      stack.push(42);
      stack.push(44);
      stack.push(43);
    };
    // starting pushing thread
    std::future<void> push_done = std::async(std::launch::async, push_action);

    // starting pop thread
    std::promise<void> pop_ready_for_test;
    auto pop_action = [&stack, ready, &pop_ready_for_test]()
    {
      pop_ready_for_test.set_value();  // reporting that we are ready to go
      ready.wait();                    // waiting for command to start the race
      std::vector<int> result;
      result.push_back(*stack.wait_and_pop());
      result.push_back(*stack.wait_and_pop());
      result.push_back(*stack.wait_and_pop());
      return result;
    };
    std::future<std::vector<int>> pop_done = std::async(std::launch::async, pop_action);

    // waiting for threads being prepared for racing
    push_ready_for_test.get_future().wait();
    pop_ready_for_test.get_future().wait();

    // starting concurrent push and pop
    go.set_value();

    // checking result
    push_done.get();  // making sure pushing thread has finished

    std::vector<int> result = pop_done.get();
    std::sort(result.begin(), result.end());
    EXPECT_EQ(result, std::vector<int>({42, 43, 44}));
    EXPECT_TRUE(stack.empty());
  }
  catch (...)
  {
    go.set_value();
    throw;
  }
}

//! Explicitely terminate waiting (concurrent mode).

TEST_F(ThreadSafeStackTests, ConcurentStopWaiting)
{
  threadsafe_stack<int> stack;
  std::promise<void> go;
  std::shared_future<void> ready(go.get_future());

  try
  {
    // starting pop thread
    std::promise<void> pop_ready_for_test;
    auto pop_action = [&stack, ready, &pop_ready_for_test]()
    {
      pop_ready_for_test.set_value();
      ready.wait();
      return stack.wait_and_pop();
    };
    std::future<std::shared_ptr<int>> pop_done = std::async(std::launch::async, pop_action);

    // waiting for threads being prepared for racing
    pop_ready_for_test.get_future().wait();

    // starting waiting on empty stack
    go.set_value();

    // stopping waiting
    stack.stop();

    // stopping stack will raise exception
    EXPECT_THROW(*pop_done.get(), empty_container_exception);
    EXPECT_TRUE(stack.empty());
  }
  catch (...)
  {
    go.set_value();
    throw;
  }
}
