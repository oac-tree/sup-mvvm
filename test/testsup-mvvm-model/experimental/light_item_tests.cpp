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

#include "mvvm/experimental/light_item.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/experimental/light_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm
{

class LightItemTest : public ::testing::Test
{
public:
};

TEST_F(LightItemTest, SetData)
{
  experimental::LightItem item;

  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(item.SetData(value));
  EXPECT_EQ(item.Data(), value);
}

TEST_F(LightItemTest, SetDataViaStrategy)
{
  experimental::LightItem item;

  auto strategy = [](auto item, const variant_t& value, std::int32_t role) -> bool
  { return item->SetDataIntern(value, role); };
  item.SetDataStrategy(strategy);

  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(item.SetData(value));
  EXPECT_EQ(item.Data(), value);

  auto increment_strategy = [](auto item, const variant_t& value, std::int32_t role) -> bool
  {
    mvvm::int32 int_value = std::get<mvvm::int32>(value);
    return item->SetDataIntern(++int_value, role);
  };
  item.SetDataStrategy(increment_strategy);

  const variant_t new_value{mvvm::int32{43}};
  const variant_t expected_value{mvvm::int32{44}};
  EXPECT_TRUE(item.SetData(new_value));
  EXPECT_EQ(item.Data(), expected_value);
}

TEST_F(LightItemTest, GetRootItem)
{
  experimental::LightModel model({});
  ASSERT_NE(model.GetRootItem(), nullptr);
  EXPECT_EQ(model.GetRootItem()->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetParent(), nullptr);
}

TEST_F(LightItemTest, InsertItem)
{
  experimental::LightModel model({});

  auto child = std::make_unique<experimental::LightItem>();
  auto child_ptr = child.get();
  model.InsertItem(std::move(child), model.GetRootItem(), 0);

  ASSERT_NE(model.GetRootItem(), nullptr);
  EXPECT_EQ(model.GetRootItem()->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetParent(), nullptr);
  EXPECT_EQ(model.GetRootItem()->GetItem(0), child_ptr);
  EXPECT_EQ(model.GetRootItem()->GetItem(0)->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetItem(0)->GetParent(), model.GetRootItem());
}

TEST_F(LightItemTest, SetDataWhenPartOfTheModel)
{
  experimental::LightModel model({});

  auto child = std::make_unique<experimental::LightItem>();
  auto child_ptr = child.get();
  model.InsertItem(std::move(child), model.GetRootItem(), 0);

  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(child_ptr->SetData(value));
  EXPECT_EQ(child_ptr->Data(), value);

  // set data via model
  const variant_t new_value{mvvm::int32{42}};
  model.SetData(child_ptr, new_value, DataRole::kData);
  EXPECT_EQ(child_ptr->Data(), new_value);
}

TEST_F(LightItemTest, SetDataWhenPartOfTheModelViaSTrategy)
{
  experimental::LightModel model({});

  auto child = std::make_unique<experimental::LightItem>();
  auto child_ptr = child.get();
  model.InsertItem(std::move(child), model.GetRootItem(), 0);

  auto increment_strategy = [](auto item, const variant_t& value, std::int32_t role) -> bool
  {
    mvvm::int32 int_value = std::get<mvvm::int32>(value);
    return item->SetDataIntern(++int_value, role);
  };
  child_ptr->SetDataStrategy(increment_strategy);

  const variant_t new_value{mvvm::int32{43}};
  const variant_t expected_value{mvvm::int32{44}};

  model.SetData(child_ptr, new_value, DataRole::kData);
  EXPECT_EQ(child_ptr->Data(), expected_value);
}

TEST_F(LightItemTest, Undo)
{
  experimental::LightModel model({});

  auto child = std::make_unique<experimental::LightItem>();
  auto child_ptr = child.get();
  const variant_t value{mvvm::int32{42}};
  EXPECT_TRUE(child_ptr->SetData(value));
  EXPECT_EQ(child_ptr->Data(), value);

  model.InsertItem(std::move(child), model.GetRootItem(), 0);

  // set data via model
  const variant_t new_value{mvvm::int32{43}};
  model.SetData(child_ptr, new_value, DataRole::kData);
  EXPECT_EQ(child_ptr->Data(), new_value);

  model.Undo();
  EXPECT_EQ(child_ptr->Data(), value);

  model.Redo();
  EXPECT_EQ(child_ptr->Data(), new_value);
}

TEST_F(LightItemTest, UndoWithStrategy)
{
  experimental::LightModel model({});

  auto child0 = std::make_unique<experimental::LightItem>();
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<experimental::LightItem>();
  auto child1_ptr = child1.get();

  const variant_t value0{mvvm::int32{42}};
  EXPECT_TRUE(child0_ptr->SetData(value0));
  EXPECT_EQ(child0_ptr->Data(), value0);
  const variant_t value1{mvvm::int32{43}};
  EXPECT_TRUE(child1_ptr->SetData(value1));
  EXPECT_EQ(child1_ptr->Data(), value1);

  model.InsertItem(std::move(child0), model.GetRootItem(), 0);
  model.InsertItem(std::move(child1), model.GetRootItem(), 1);

  const variant_t new_value0{mvvm::int32{10}};
  const variant_t new_value1{mvvm::int32{20}};

  // strategy set the value for first item, and doubles the value for the second item
  auto strategy = [&model, child1_ptr](auto item, const variant_t& value, std::int32_t role) -> bool
  {
    model.GetCommandStack()->BeginMacro("macro");

    auto result = item->SetDataIntern(value, role);
    mvvm::int32 int_value = std::get<mvvm::int32>(value) * 2;  // doubles the value
    child1_ptr->SetData(int_value);

    model.GetCommandStack()->EndMacro();
    return result;
  };
  child0_ptr->SetDataStrategy(strategy);

  model.SetData(child0_ptr, new_value0, DataRole::kData);
  EXPECT_EQ(child0_ptr->Data(), new_value0);
  EXPECT_EQ(child1_ptr->Data(), new_value1);

  model.Undo();
  EXPECT_EQ(child0_ptr->Data(), value0);
  EXPECT_EQ(child1_ptr->Data(), value1);

  model.Redo();
  EXPECT_EQ(child0_ptr->Data(), new_value0);
  EXPECT_EQ(child1_ptr->Data(), new_value1);
}

}  // namespace mvvm
