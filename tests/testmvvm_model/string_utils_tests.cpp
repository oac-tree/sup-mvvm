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

#include "mvvm/utils/string_utils.h"

#include <gtest/gtest.h>

namespace
{
//! Creates vector of T from argument list. Used in EXPECT_EQ macros for convenience.

template <typename T = std::string, typename... Args>
std::vector<T> toVector(Args&&... args)
{
  std::vector<T> v;
  (v.push_back(T(args)), ...);
  return v;
}

}  // namespace

using namespace mvvm;

//! Testing strint related utility functions.

class StringUtilsTests : public ::testing::Test
{
};

//! Testing function DoubleToString.

TEST_F(StringUtilsTests, DoubleToString)
{
  using utils::DoubleToString;
  const int precision = 4;
  EXPECT_EQ(DoubleToString(0.0, precision), "0.0");
  EXPECT_EQ(DoubleToString(1.001, precision), "1.001");
  EXPECT_EQ(DoubleToString(1.0001, precision), "1.0");
}

//! Testing function ScientificDoubleToString.

TEST_F(StringUtilsTests, ScientificDoubleToString)
{
  using utils::ScientificDoubleToString;
  const int precision = 6;
  EXPECT_EQ(ScientificDoubleToString(0.0, precision), "0.0e+00");
  EXPECT_EQ(ScientificDoubleToString(1.0, precision), "1.0e+00");
  EXPECT_EQ(ScientificDoubleToString(3.0 / 4.0, precision), "7.5e-01");
  EXPECT_EQ(ScientificDoubleToString(4.0 / 3.0, precision), "1.333333e+00");
  EXPECT_EQ(ScientificDoubleToString(1000000., precision), "1.0e+06");
}

//! Testing function TrimWhitespace

TEST_F(StringUtilsTests, TrimWhiteSpace)
{
  using utils::TrimWhitespace;
  EXPECT_EQ(TrimWhitespace(""), std::string());
  EXPECT_EQ(TrimWhitespace(" "), std::string());
  EXPECT_EQ(TrimWhitespace("abc"), std::string("abc"));
  EXPECT_EQ(TrimWhitespace(" \t\n abc cde\n"), std::string("abc cde"));
}

//! Testing function StringToBool.

TEST_F(StringUtilsTests, StringToBool)
{
  using utils::StringToBool;

  EXPECT_TRUE(StringToBool("true"));
  EXPECT_FALSE(StringToBool("false"));

  EXPECT_TRUE(StringToBool("True"));
  EXPECT_FALSE(StringToBool("False"));

  EXPECT_TRUE(StringToBool("True "));
  EXPECT_FALSE(StringToBool(" False"));

  EXPECT_THROW(StringToBool("abc "), std::runtime_error);
}

TEST_F(StringUtilsTests, FromBool)
{
  using utils::FromBool;

  EXPECT_EQ(FromBool(true), std::string("true"));
  EXPECT_EQ(FromBool(false), std::string("false"));
}

//! Testing function StringToDouble.

TEST_F(StringUtilsTests, StringToDouble)
{
  using utils::StringToDouble;

  // not a double
  EXPECT_FALSE(StringToDouble("").has_value());
  EXPECT_FALSE(StringToDouble(" ").has_value());
  EXPECT_FALSE(StringToDouble("a").has_value());
  EXPECT_FALSE(StringToDouble("a b").has_value());

  // not a double: some mixture present
  EXPECT_FALSE(StringToDouble("42a").has_value());
  EXPECT_FALSE(StringToDouble("42.5.5").has_value());

  // not a double: more than one double
  EXPECT_FALSE(StringToDouble("42.5 52").has_value());

  // valid double
  EXPECT_TRUE(StringToDouble("42").has_value());
  EXPECT_TRUE(StringToDouble(" 42").has_value());
  EXPECT_TRUE(StringToDouble(" 42 ").has_value());
  EXPECT_DOUBLE_EQ(StringToDouble("42").value(), 42.0);
  EXPECT_TRUE(StringToDouble("42.5").has_value());
  EXPECT_DOUBLE_EQ(StringToDouble("42.5").value(), 42.5);
  EXPECT_TRUE(StringToDouble("-1.12e-06").has_value());
  EXPECT_DOUBLE_EQ(StringToDouble("-1.12e-06").value(), -1.12e-06);
}

//! Testing function StringToDouble.

TEST_F(StringUtilsTests, StringToInteger)
{
  using utils::StringToInteger;

  // not an int
  EXPECT_FALSE(StringToInteger("").has_value());
  EXPECT_FALSE(StringToInteger(" ").has_value());
  EXPECT_FALSE(StringToInteger("a").has_value());
  EXPECT_FALSE(StringToInteger("a b").has_value());

  // not an int: some mixture present
  EXPECT_FALSE(StringToInteger("42a").has_value());
  EXPECT_FALSE(StringToInteger("42.5").has_value());

  // not an int: more than one number
  EXPECT_FALSE(StringToInteger("42.5 52").has_value());

  // valid int
  EXPECT_TRUE(StringToInteger("42").has_value());
  EXPECT_TRUE(StringToInteger(" 42").has_value());
  EXPECT_TRUE(StringToInteger(" 42 ").has_value());
  EXPECT_EQ(StringToInteger("42").value(), 42);
}

//! Testing SplitString method.
//! Carefully checking that it is reproduces Python behavior.

TEST_F(StringUtilsTests, SplitString)
{
  using utils::SplitString;
  using vec_t = std::vector<std::string>;

  EXPECT_THROW(SplitString("", ""), std::runtime_error);
  EXPECT_EQ(SplitString("", " "), toVector());
  EXPECT_EQ(SplitString("", ","), toVector());
  EXPECT_EQ(SplitString(" ", ","), toVector(" "));
  EXPECT_EQ(SplitString(" ", " "), toVector("", ""));
  EXPECT_EQ(SplitString("a", " "), toVector("a"));
  EXPECT_EQ(SplitString("a ", " "), toVector("a", ""));

  EXPECT_EQ(SplitString("a b", " "), toVector("a", "b"));
  EXPECT_EQ(SplitString("a  b", " "), toVector("a", "", "b"));

  EXPECT_EQ(SplitString("a", "-"), toVector("a"));

  EXPECT_EQ(SplitString("aa", "a"), toVector("", "", ""));

  EXPECT_EQ(SplitString("a,b", ","), toVector("a", "b"));
  EXPECT_EQ(SplitString("a, b", ","), toVector("a", " b"));

  EXPECT_EQ(SplitString("a,b,", ","), toVector("a", "b", ""));
  EXPECT_EQ(SplitString(",a,b,", ","), toVector("", "a", "b", ""));
  EXPECT_EQ(SplitString("aabbcc", "bb"), toVector("aa", "cc"));
  EXPECT_EQ(SplitString("aabbcc", "bb"), toVector("aa", "cc"));

  // ; delimeter
  EXPECT_EQ(SplitString(";", ";"), toVector("", ""));
  EXPECT_EQ(SplitString(";;", ";"), toVector("", "", ""));
  EXPECT_EQ(SplitString(";aaa bbb;", ";"), toVector("", "aaa bbb", ""));
}

//! Testing ParseSpaceSeparatedDoubles.
//! This is a fast method which forgives malformed doubles.

TEST_F(StringUtilsTests, ParseSpaceSeparatedDoubles)
{
  using utils::ParseSpaceSeparatedDoubles;
  std::vector<double> data;

  EXPECT_TRUE(ParseSpaceSeparatedDoubles("").empty());
  EXPECT_TRUE(ParseSpaceSeparatedDoubles(" ").empty());
  EXPECT_TRUE(ParseSpaceSeparatedDoubles("a").empty());
  EXPECT_TRUE(ParseSpaceSeparatedDoubles("a b").empty());

  ASSERT_EQ(ParseSpaceSeparatedDoubles("4.02").size(), 1);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42")[0], 42.0);

  // this tests failing under MacOS
  //    ASSERT_EQ(ParseSpaceSeparatedDoubles("42aaa").size(), 1u);
  //    EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42aaa")[0], 42.0);

  EXPECT_EQ(ParseSpaceSeparatedDoubles("42,").size(), 1);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42,")[0], 42.0);

  EXPECT_EQ(ParseSpaceSeparatedDoubles("42,43").size(), 1);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42,43")[0], 42.0);

  EXPECT_EQ(ParseSpaceSeparatedDoubles("42 ,43").size(), 1);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42 ,43")[0], 42.0);

  EXPECT_EQ(ParseSpaceSeparatedDoubles("42 43").size(), 2);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42 43")[0], 42.0);
  EXPECT_DOUBLE_EQ(ParseSpaceSeparatedDoubles("42 43")[1], 43.0);
}

TEST_F(StringUtilsTests, ParseCommaSeparatedDoubles)
{
  using utils::ParseCommaSeparatedDoubles;

  EXPECT_TRUE(ParseCommaSeparatedDoubles("").empty());
  EXPECT_TRUE(ParseCommaSeparatedDoubles(" ").empty());
  EXPECT_EQ(ParseCommaSeparatedDoubles("1.0"), toVector<double>(1.0));
  EXPECT_EQ(ParseCommaSeparatedDoubles("1.0, 2.0"), toVector<double>(1.0, 2.0));
  EXPECT_EQ(ParseCommaSeparatedDoubles("1.0,2.0"), toVector<double>(1.0, 2.0));
  EXPECT_THROW(ParseCommaSeparatedDoubles(", 1.0"), std::runtime_error);
}

TEST_F(StringUtilsTests, ToComaSeparatedString)
{
  using utils::ToCommaSeparatedString;

  EXPECT_EQ(ToCommaSeparatedString(std::vector<double>({})), std::string());
  EXPECT_EQ(ToCommaSeparatedString(std::vector<double>({1.0})), std::string("1.0"));
  EXPECT_EQ(ToCommaSeparatedString(std::vector<double>({1.0, 2.0})), std::string("1.0, 2.0"));

  EXPECT_EQ(ToCommaSeparatedString(std::vector<std::string>({})), std::string());
  EXPECT_EQ(ToCommaSeparatedString(std::vector<std::string>({"abc"})), std::string("abc"));
  EXPECT_EQ(ToCommaSeparatedString(std::vector<std::string>({"abc", "cde"})),
            std::string("abc, cde"));
}
