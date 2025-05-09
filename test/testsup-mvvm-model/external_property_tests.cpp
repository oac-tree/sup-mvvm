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

#include "mvvm/model/external_property.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ExternalPropertyTests : public ::testing::Test
{
};

TEST_F(ExternalPropertyTests, InitialState)
{
  ExternalProperty property;
  EXPECT_TRUE(property.IsEmpty());
  EXPECT_TRUE(property.GetText().empty());
  EXPECT_TRUE(property.GetIdentifier().empty());
  EXPECT_TRUE(property.GetColorName().empty());
}

TEST_F(ExternalPropertyTests, Constructor)
{
  ExternalProperty property("text", "red", "123");
  EXPECT_FALSE(property.IsEmpty());
  EXPECT_EQ(property.GetText(), "text");
  EXPECT_EQ(property.GetColorName(), "red");
  EXPECT_EQ(property.GetIdentifier(), "123");
}

TEST_F(ExternalPropertyTests, EqualityOperators)
{
  ExternalProperty prop1a;
  ExternalProperty prop1b;

  EXPECT_TRUE(prop1a == prop1b);
  EXPECT_FALSE(prop1a < prop1b);

  ExternalProperty prop2a("text", "red");
  ExternalProperty prop2b("text", "red");
  EXPECT_TRUE(prop2a == prop2b);
  EXPECT_FALSE(prop2a < prop2b);

  EXPECT_FALSE(prop1a == prop2a);
}

TEST_F(ExternalPropertyTests, ConvertToString)
{
  EXPECT_EQ(ExternalProperty().ToString(), std::string(";;"));

  EXPECT_EQ(ExternalProperty("text", "color", "identifier").ToString(),
            std::string("text;color;identifier"));

  EXPECT_EQ(ExternalProperty::CreateFromString(";;"), ExternalProperty());
  EXPECT_EQ(ExternalProperty::CreateFromString("text;color;identifier"),
            ExternalProperty("text", "color", "identifier"));
}
