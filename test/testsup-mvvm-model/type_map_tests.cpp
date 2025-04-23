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

#include "mvvm/core/type_map.h"

#include <gtest/gtest.h>

#include <string>

using namespace mvvm;

//! Testing TypeMap class.

class TypeMapTests : public ::testing::Test
{
};

TEST_F(TypeMapTests, TypeMapTestsPut)
{
  TypeMap<std::string> type_map;

  auto int_id = type_map.GetTypeId<int>();
  auto string_id = type_map.GetTypeId<std::string>();
  EXPECT_EQ(int_id, type_map.GetTypeId<int>());
  EXPECT_EQ(string_id, type_map.GetTypeId<std::string>());

  type_map.Put<int>("integer");
  type_map.Put<std::string>("string");

  EXPECT_EQ(int_id, type_map.GetTypeId<int>());
  EXPECT_EQ(string_id, type_map.GetTypeId<std::string>());

  auto it = type_map.Find<int>();
  EXPECT_NE(it, type_map.end());
  EXPECT_EQ(it->second, "integer");
  it = type_map.Find<std::string>();
  EXPECT_NE(it, type_map.end());
  EXPECT_EQ(it->second, "string");
}
