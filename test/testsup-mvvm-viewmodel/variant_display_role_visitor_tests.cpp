/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/providers/variant_display_role_visitor.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for VariantDisplayRoleVisitor class.
 */
class VariantDisplayRoleVisitorTest : public ::testing::Test
{
};

TEST_F(VariantDisplayRoleVisitorTest, Visit)
{
  const role_t main_data_role = DataRole::kData;

  {  // empty
    SessionItem item;
    item.SetData(variant_t{}, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_FALSE(visitor.ToString().has_value());
  }

  {  // boolean True
    SessionItem item;
    item.SetData(true, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("True"));
  }

  {  // boolean False
    SessionItem item;
    item.SetData(false, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("False"));
  }

  {  // char8
    SessionItem item;
    item.SetData(static_cast<mvvm::char8>('a'), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("97"));  // ascii code
  }

  {  // int8
    SessionItem item;
    item.SetData(static_cast<mvvm::int8>(-8), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-8"));
  }

  {  // uint8
    SessionItem item;
    item.SetData(static_cast<mvvm::uint8>(16), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("16"));
  }

  {  // int16
    SessionItem item;
    item.SetData(static_cast<mvvm::int16>(-42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint16
    SessionItem item;
    item.SetData(static_cast<mvvm::uint16>(42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // int32
    SessionItem item;
    item.SetData(static_cast<mvvm::int32>(-42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint32
    SessionItem item;
    item.SetData(static_cast<mvvm::uint32>(42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // int64
    SessionItem item;
    item.SetData(static_cast<mvvm::int64>(-42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint64
    SessionItem item;
    item.SetData(static_cast<mvvm::uint64>(42), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // float32
    SessionItem item;
    item.SetData(static_cast<mvvm::float32>(48.0), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("48.0000"));
  }

  {  // float32 with 5 digits after decimal point
    SessionItem item;
    item.SetData(static_cast<mvvm::float32>(48.12345), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    // 4 digits after decimal point by default
    EXPECT_EQ(visitor.ToString(), std::string("48.1235"));
  }

  {  // float64
    SessionItem item;
    item.SetData(static_cast<mvvm::float64>(48.0), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("48.0000"));
  }

  {  // string
    SessionItem item;
    item.SetData(std::string("abc"), main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("abc"));
  }

  {  // vector of double
    SessionItem item;
    item.SetData(std::vector<double>{1.0, 2.0, 3.0}, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("1.0,2.0,3.0"));
  }

  {  // ComboProperty
    SessionItem item;
    const ComboProperty value = ComboProperty::CreateFrom(std::vector<std::string>{"a1", "a2"});
    item.SetData(value, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("a1"));
  }

  {  // ExternalProperty
    SessionItem item;
    const ExternalProperty value("text", "color", "identifier");
    item.SetData(value, main_data_role);
    const VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("text"));
  }
}

//! Float data with precision and notation of the item.
TEST_F(VariantDisplayRoleVisitorTest, FloatPrecisionAndNotation)
{
  const role_t main_data_role = DataRole::kData;

  SessionItem item;
  item.SetData(4.4647e-34, main_data_role);
  EXPECT_EQ(VariantDisplayRoleVisitor(item, main_data_role).ToString(), std::string("0.0000"));

  item.SetFlag(Appearance::kScientific, true);
  EXPECT_EQ(VariantDisplayRoleVisitor(item, main_data_role).ToString(), std::string("4.4647e-34"));

  item.SetData(std::int32_t{2}, DataRole::kPrecision);
  EXPECT_EQ(VariantDisplayRoleVisitor(item, main_data_role).ToString(), std::string("4.46e-34"));

  item.SetFlag(Appearance::kScientific, false);
  item.SetData(1.5, main_data_role);
  EXPECT_EQ(VariantDisplayRoleVisitor(item, main_data_role).ToString(), std::string("1.50"));

  SessionItem float32_item;
  float32_item.SetData(static_cast<mvvm::float32>(1.5), main_data_role);
  float32_item.SetData(std::int32_t{2}, DataRole::kPrecision);
  EXPECT_EQ(VariantDisplayRoleVisitor(float32_item, main_data_role).ToString(),
            std::string("1.50"));
}

}  // namespace mvvm::test
