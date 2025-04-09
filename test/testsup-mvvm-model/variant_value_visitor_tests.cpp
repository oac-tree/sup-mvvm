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

#include "mvvm/core/variant_value_visitor.h"

#include <mvvm/utils/string_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Test of VariantValueVisitor struct.
 */
class VariantValueVisitorTests : public ::testing::Test
{
};

TEST_F(VariantValueVisitorTests, Visit)
{
  VariantValueVisitor visitor;

  {
    const std::monostate value;
    EXPECT_EQ(visitor.operator()(value), std::string());
  }

  {
    const mvvm::boolean value{true};
    EXPECT_EQ(visitor.operator()(value), std::string("true"));
  }

  {
    const mvvm::char8 value{'a'};
    EXPECT_EQ(visitor.operator()(value), std::string("97"));  // ascii code
  }

  {  // special case, null char
    const mvvm::char8 value{'\0'};
    EXPECT_EQ(visitor.operator()(value), std::string("0"));  // ascii code
  }

  {
    const mvvm::int8 value{-8};
    EXPECT_EQ(visitor.operator()(value), std::string("-8"));
  }

  {
    const mvvm::uint8 value{16};
    EXPECT_EQ(visitor.operator()(value), std::string("16"));
  }

  {
    const mvvm::int16 value{-42};
    EXPECT_EQ(visitor.operator()(value), std::string("-42"));
  }

  {
    const mvvm::uint16 value{42};
    EXPECT_EQ(visitor.operator()(value), std::string("42"));
  }

  {
    const mvvm::int32 value{-42};
    EXPECT_EQ(visitor.operator()(value), std::string("-42"));
  }

  {
    const mvvm::uint32 value{42};
    EXPECT_EQ(visitor.operator()(value), std::string("42"));
  }

  {
    const mvvm::int64 value{-42};
    EXPECT_EQ(visitor.operator()(value), std::string("-42"));
  }

  {
    const mvvm::uint64 value{42};
    EXPECT_EQ(visitor.operator()(value), std::string("42"));
  }

  {
    const mvvm::float32 value{48.0};
    EXPECT_EQ(visitor.operator()(value), std::string("48.0"));
  }

  {
    const mvvm::float64 value{48.0};
    EXPECT_EQ(visitor.operator()(value), std::string("48.0"));
  }

  {
    const std::string value{"abc"};
    EXPECT_EQ(visitor.operator()(value), std::string("abc"));
  }

  {
    const std::vector<double> value{1.0, 2.0, 3.0};
    EXPECT_EQ(visitor.operator()(value), std::string("1.0, 2.0, 3.0"));
  }

  {
    const ComboProperty value = ComboProperty::CreateFrom(std::vector<std::string>{"a1", "a2"});
    EXPECT_EQ(visitor.operator()(value), std::string("a1;a2"));
  }

  {
    const ExternalProperty value("text", "color", "identifier");
    EXPECT_EQ(visitor.operator()(value), std::string("text;color;identifier"));
  }
}
