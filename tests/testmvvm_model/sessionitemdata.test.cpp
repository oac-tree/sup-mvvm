// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/model/sessionitemdata.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace ModelView;

//! Test of SessionItemData.

class SessionItemDataTest : public ::testing::Test
{
};

//! Initial state of SessionItemData object.

TEST_F(SessionItemDataTest, initialState)
{
  SessionItemData item_data;
  EXPECT_TRUE(item_data.roles().empty());
  EXPECT_FALSE(item_data.hasData(0));
  EXPECT_FALSE(Utils::IsValid(item_data.data(0)));
}

//! Basic setData, data operations.

TEST_F(SessionItemDataTest, setDataDouble)
{
  const int role = 1;
  SessionItemData item_data;

  variant_t variant(42.0);

  // setting variant for role
  EXPECT_TRUE(item_data.setData(variant, role));
  std::vector<int> expected_roles{role};
  EXPECT_EQ(item_data.roles(), expected_roles);
  EXPECT_TRUE(item_data.data(role) == variant);

  // setting same data twice
  EXPECT_FALSE(item_data.setData(variant, role));
  EXPECT_EQ(item_data.roles(), expected_roles);
  EXPECT_TRUE(item_data.data(role) == variant);

  // changing the data
  EXPECT_TRUE(item_data.setData(variant_t(43.0), role));
  EXPECT_EQ(item_data.roles(), expected_roles);
  EXPECT_TRUE(item_data.data(role) == variant_t(43.0));

  // setting invalid variant for the role will remove data
  EXPECT_TRUE(item_data.setData(variant_t(), role));
  EXPECT_TRUE(item_data.roles().empty());
  EXPECT_FALSE(Utils::IsValid(item_data.data(role)));
}

//! Using different roles.

TEST_F(SessionItemDataTest, differentRoles)
{
  SessionItemData item_data;

  const int role1(1);
  const int role2 = role1 + 1;

  EXPECT_TRUE(item_data.setData(variant_t(42.0), role1));
  EXPECT_TRUE(item_data.setData(variant_t(std::string("str")), role2));

  std::vector<int> expected{role1, role2};
  EXPECT_EQ(item_data.roles(), expected);

  EXPECT_TRUE(item_data.data(role1) == variant_t(42.0));
  EXPECT_TRUE(item_data.data(role2) == variant_t(std::string("str")));
  EXPECT_FALSE(item_data.data(role2) == variant_t(42.0));
  EXPECT_FALSE(item_data.data(role1) == variant_t(std::string("str")));
}

//! Changing type of variant for role should not be allowed.

TEST_F(SessionItemDataTest, changingRole)
{
  SessionItemData item_data;

  const int role(1);
  const variant_t variant(42.0);

  // setting variant for role
  EXPECT_TRUE(item_data.setData(variant, role));
  std::vector<int> expected{role};
  EXPECT_EQ(item_data.roles(), expected);
  EXPECT_TRUE(item_data.data(role) == variant);

  variant_t s = std::string("str");
  EXPECT_THROW(item_data.setData(s, role), std::runtime_error);
}

//! Access to data and roles via range loop.

TEST_F(SessionItemDataTest, rangeLoop)
{
  SessionItemData item_data;
  const std::vector<double> expected_values = {1.2, 1.3};
  const std::vector<int> expected_roles = {1, 2};

  for (size_t i = 0; i < expected_values.size(); ++i)
  {
    item_data.setData(expected_values[i], expected_roles[i]);
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

TEST_F(SessionItemDataTest, hasRole)
{
  SessionItemData data;
  EXPECT_FALSE(data.hasData(0));
  EXPECT_FALSE(data.hasData(1));

  const int role = 99;
  data.setData(variant_t(42), role);
  EXPECT_TRUE(data.hasData(role));
  EXPECT_FALSE(data.hasData(1));

  data.setData(variant_t(), role);
  EXPECT_FALSE(data.hasData(role));
}
