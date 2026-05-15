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
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string());
  }

  {  // boolean
    SessionItem item;
    item.SetData(true, main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("true"));
  }

  {  // char8
    SessionItem item;
    item.SetData(static_cast<mvvm::char8>('a'), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("97"));  // ascii code
  }

  {  // int8
    SessionItem item;
    item.SetData(static_cast<mvvm::int8>(-8), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-8"));
  }

  {  // uint8
    SessionItem item;
    item.SetData(static_cast<mvvm::uint8>(16), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("16"));
  }

  {  // int16
    SessionItem item;
    item.SetData(static_cast<mvvm::int16>(-42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint16
    SessionItem item;
    item.SetData(static_cast<mvvm::uint16>(42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // int32
    SessionItem item;
    item.SetData(static_cast<mvvm::int32>(-42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint32
    SessionItem item;
    item.SetData(static_cast<mvvm::uint32>(42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // int64
    SessionItem item;
    item.SetData(static_cast<mvvm::int64>(-42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("-42"));
  }

  {  // uint64
    SessionItem item;
    item.SetData(static_cast<mvvm::uint64>(42), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("42"));
  }

  {  // float32
    SessionItem item;
    item.SetData(static_cast<mvvm::float32>(48.0), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("48.0"));
  }

  {  // float64
    SessionItem item;
    item.SetData(static_cast<mvvm::float64>(48.0), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("48.0"));
  }

  {  // string
    SessionItem item;
    item.SetData(std::string("abc"), main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("abc"));
  }

  {  // vector of double
    SessionItem item;
    item.SetData(std::vector<double>{1.0, 2.0, 3.0}, main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("1.0,2.0,3.0"));
  }

  {  // ComboProperty
    SessionItem item;
    const ComboProperty value = ComboProperty::CreateFrom(std::vector<std::string>{"a1", "a2"});
    item.SetData(value, main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("values[a1;a2] selection[0]"));
  }

  {  // ExternalProperty
    SessionItem item;
    const ExternalProperty value("text", "color", "identifier");
    item.SetData(value, main_data_role);
    VariantDisplayRoleVisitor visitor(item, main_data_role);
    EXPECT_EQ(visitor.ToString(), std::string("text;color;identifier"));
  }
}

}  // namespace mvvm::test
