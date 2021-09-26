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

#include "mvvm/experimental/anyvalue.h"

#include <gtest/gtest.h>

//! Testing AnyValue class.

using Experimental::AnyValue;

class AnyValueTest : public ::testing::Test
{
};

//! Checking equality operator (necessary for other checks).

TEST_F(AnyValueTest, DataType)
{
  EXPECT_EQ(AnyValue::data_t(42), AnyValue::data_t(42));
}

//! Simple property creation and accessors.

TEST_F(AnyValueTest, SimpleInitialization)
{
  AnyValue value;

  value.Add("status", 42);
  value.Add("description", "abc");

  // access via index operator
  EXPECT_EQ(std::get<int>(value["status"]), 42);
  EXPECT_EQ(std::get<std::string>(value["description"]), "abc");

  // via getter with type conversion
  EXPECT_EQ(value.Data<int>("status"), 42);
  EXPECT_EQ(value.Data<std::string>("description"), "abc");

  // same as before, with variant comparison
  EXPECT_EQ(value.Data("status"), AnyValue::data_t(42));
  EXPECT_EQ(value.Data("description"), AnyValue::data_t("abc"));

  // attempt to access wrong type (should be string, not int)
  EXPECT_THROW(value.Data<int>("description"), std::exception);
}

TEST_F(AnyValueTest, InitialiserList)
{
  AnyValue value = {{"int", 42}, {"double", 3.14}, {"string", "abc"}};

  EXPECT_EQ(value.Data<int>("int"), 42);
  EXPECT_EQ(value.Data<double>("double"), 3.14);
  EXPECT_EQ(value.Data<std::string>("string"), "abc");
}

TEST_F(AnyValueTest, NestedStruct)
{
  AnyValue value1("status", 42);

  AnyValue value2;
  value2.Add("struct", value1);

  // not very elegant, but probably can be improved
  EXPECT_EQ(value2.Data<AnyValue>("struct").Data<int>("status"), 42);
}

//! Same test as before with initialiser list initialization.

TEST_F(AnyValueTest, InitialiserListWithNestedStructures)
{
  AnyValue value2 = {{"struct", AnyValue{"status", 42}}};
  EXPECT_EQ(value2.Data<AnyValue>("struct").Data<int>("status"), 42);
}
