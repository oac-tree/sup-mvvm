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

#include "mvvm/model/sessionitem_data.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>

using namespace mvvm;

//! Test of SessionItemData.

class SessionItemDataTests : public ::testing::Test
{
};

//! Initial state of SessionItemData object.

TEST_F(SessionItemDataTests, initialState)
{
  SessionItemData item_data;
  EXPECT_TRUE(item_data.GetRoles().empty());
  EXPECT_FALSE(item_data.HasData(0));
  EXPECT_FALSE(utils::IsValid(item_data.Data(0)));
}

//! Basic setData, data operations.

TEST_F(SessionItemDataTests, SetDataDouble)
{
  const int role = 1;
  SessionItemData item_data;

  variant_t variant(42.0);

  // setting variant for role
  EXPECT_TRUE(item_data.SetData(variant, role));
  std::vector<int> expected_roles{role};
  EXPECT_EQ(item_data.GetRoles(), expected_roles);
  EXPECT_TRUE(item_data.Data(role) == variant);

  // setting same data twice
  EXPECT_FALSE(item_data.SetData(variant, role));
  EXPECT_EQ(item_data.GetRoles(), expected_roles);
  EXPECT_TRUE(item_data.Data(role) == variant);

  // changing the data
  EXPECT_TRUE(item_data.SetData(variant_t(43.0), role));
  EXPECT_EQ(item_data.GetRoles(), expected_roles);
  EXPECT_TRUE(item_data.Data(role) == variant_t(43.0));

  // setting invalid variant for the role will remove data
  EXPECT_TRUE(item_data.SetData(variant_t(), role));
  EXPECT_TRUE(item_data.GetRoles().empty());
  EXPECT_FALSE(utils::IsValid(item_data.Data(role)));
}

//! Using different roles.

TEST_F(SessionItemDataTests, DifferentRoles)
{
  SessionItemData item_data;

  const int role1(1);
  const int role2 = role1 + 1;

  EXPECT_TRUE(item_data.SetData(variant_t(42.0), role1));
  EXPECT_TRUE(item_data.SetData(variant_t(std::string("str")), role2));

  std::vector<int> expected{role1, role2};
  EXPECT_EQ(item_data.GetRoles(), expected);

  EXPECT_TRUE(item_data.Data(role1) == variant_t(42.0));
  EXPECT_TRUE(item_data.Data(role2) == variant_t(std::string("str")));
  EXPECT_FALSE(item_data.Data(role2) == variant_t(42.0));
  EXPECT_FALSE(item_data.Data(role1) == variant_t(std::string("str")));
}

//! Changing type of variant for role should not be allowed.

TEST_F(SessionItemDataTests, ChangingRole)
{
  SessionItemData item_data;

  const int role(1);
  const variant_t variant(42.0);

  // setting variant for role
  EXPECT_TRUE(item_data.SetData(variant, role));
  std::vector<int> expected{role};
  EXPECT_EQ(item_data.GetRoles(), expected);
  EXPECT_TRUE(item_data.Data(role) == variant);

  variant_t s = std::string("str");
  EXPECT_THROW(item_data.SetData(s, role), RuntimeException);
}

//! Access to data and roles via range loop.

TEST_F(SessionItemDataTests, RangeLoop)
{
  SessionItemData item_data;
  const std::vector<double> expected_values = {1.2, 1.3};
  const std::vector<int> expected_roles = {1, 2};

  for (size_t i = 0; i < expected_values.size(); ++i)
  {
    item_data.SetData(expected_values[i], expected_roles[i]);
  }

  std::vector<double> values;
  std::vector<int> roles;

  for (const auto& [data, role] : item_data)
  {
    values.push_back(std::get<double>(data));
    roles.push_back(role);
  }

  EXPECT_EQ(values, expected_values);
  EXPECT_EQ(roles, expected_roles);
}

TEST_F(SessionItemDataTests, HasRole)
{
  SessionItemData data;
  EXPECT_FALSE(data.HasData(0));
  EXPECT_FALSE(data.HasData(1));

  const int role = 99;
  data.SetData(variant_t(42), role);
  EXPECT_TRUE(data.HasData(role));
  EXPECT_FALSE(data.HasData(1));

  data.SetData(variant_t(), role);
  EXPECT_FALSE(data.HasData(role));
}
