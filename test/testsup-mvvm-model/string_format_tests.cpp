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

#include "mvvm/utils/string_format.h"

#include <gtest/gtest.h>
using namespace mvvm;

//! Testing StringFormat class.

class StringFormatTests : public ::testing::Test
{
};

TEST_F(StringFormatTests, Replace)
{
  // Explicitely create value to invoke cast operator. The auto will not work.
  std::string value;

  value = utils::StringFormat("%1").arg("abc");
  EXPECT_EQ(value, std::string("abc"));

  value = utils::StringFormat("%1 %2").arg("abc");
  EXPECT_EQ(value, std::string("abc %2"));

  value = utils::StringFormat("%1 %2").arg("abc").arg("def");
  EXPECT_EQ(value, std::string("abc def"));

  value = utils::StringFormat("%2 %1").arg("abc").arg("def");
  EXPECT_EQ(value, std::string("def abc"));

  const std::string body{R"(<Variant role="%1" type="%2">%3</Variant>)"};

  value = utils::StringFormat(body).arg("42").arg("int32").arg("44");

  const std::string expected{R"(<Variant role="42" type="int32">44</Variant>)"};
  EXPECT_EQ(value, expected);
}
