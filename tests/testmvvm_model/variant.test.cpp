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

#include "mvvm/core/variant.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing variant_t and utility functions.

class VariantTest : public ::testing::Test
{
};

//! Testing Utils::IsValid function.

TEST_F(VariantTest, IsValid)
{
  using utils::IsValid;
  EXPECT_FALSE(IsValid(variant_t()));
  EXPECT_TRUE(IsValid(variant_t(true)));
  EXPECT_TRUE(IsValid(variant_t(42)));
  EXPECT_TRUE(IsValid(variant_t(42.1)));
  EXPECT_TRUE(IsValid(variant_t(std::string("abc"))));
  EXPECT_TRUE(IsValid(variant_t(std::vector<double>({1.0, 1.1, 1.2}))));
  EXPECT_TRUE(IsValid(variant_t(ComboProperty::CreateFrom({"a1"}))));
  EXPECT_TRUE(IsValid(variant_t(ExternalProperty("text", "color"))));
}

//! Special case of const char
//! Test is failing on old compilers: variant constructed from "const char" is turning into
//! variant<bool>

// TEST_F(VariantTest, ConstChar)
//{
//   variant_t variant1("abc");
//   variant_t variant2(std::string("abc"));
//   EXPECT_TRUE(variant1 == variant2);
//   EXPECT_EQ(std::get<std::string>(variant1), std::string("abc"));
// }

//! Vector<double>

TEST_F(VariantTest, VectorOfDouble)
{
  variant_t variant1(std::vector<double>({1.0, 2.0}));
  variant_t variant2(std::vector<double>({1.0, 2.0}));
  EXPECT_TRUE(variant1 == variant2);

  // to make sure that float precision doesn't pop-up with such kind of comparison
  EXPECT_EQ(std::get<std::vector<double>>(variant1), std::vector<double>({1.0, 2.0}));
}

TEST_F(VariantTest, ComboPropertyVariantEquality)
{
  ComboProperty c1 = ComboProperty() << "a1"
                                     << "a2";
  ComboProperty c2 = ComboProperty() << "a1"
                                     << "a2";

  EXPECT_TRUE(variant_t(c1) == variant_t(c2));

  // make c2 different
  c2 << "a3";
  c2.SetValue("a2");
  EXPECT_TRUE(variant_t(c1) != variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c2));

  // make c1 the same
  c1 << "a3";
  c1.SetValue("a2");
  EXPECT_TRUE(variant_t(c1) == variant_t(c2));
  EXPECT_FALSE(variant_t(c1) != variant_t(c2));

  // change what is selected to make them different again
  c1.SetStringOfSelections("0");
  c2.SetStringOfSelections("1");
  EXPECT_TRUE(variant_t(c1) != variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c2));
}

TEST_F(VariantTest, ExternalPropertyVariantEquality)
{
  ExternalProperty c1("text", "color", "identifier");
  ExternalProperty c2("text", "color", "identifier");
  ExternalProperty c3("text", "color", "");

  EXPECT_TRUE(variant_t(c1) == variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c3));
}

//! Testing Utils::AreCompatible function.

TEST_F(VariantTest, AreCompatible)
{
  std::vector<variant_t> variants = {variant_t(true),
                                     variant_t(42),
                                     variant_t(42.1),
                                     variant_t(std::string("abc")),
                                     variant_t(std::vector<double>({1.1, 2.2, 3.3})),
                                     variant_t(ComboProperty::CreateFrom({"a1"})),
                                     variant_t(ExternalProperty("text", "color"))};
  for (size_t i = 0; i < variants.size(); ++i)
  {
    EXPECT_TRUE(utils::AreCompatible(variant_t(), variants[i]));
    for (size_t j = 0; j < variants.size(); ++j)
    {
      if (i == j)
      {
        EXPECT_TRUE(utils::AreCompatible(variants[i], variants[j]));
      }
      else
      {
        EXPECT_FALSE(utils::AreCompatible(variants[i], variants[j]));
      }
    }
  }
}

TEST_F(VariantTest, TypeName)
{
  using utils::TypeName;
  EXPECT_EQ(TypeName(variant_t()), constants::kUndefinedTypeName);
  EXPECT_EQ(TypeName(variant_t(true)), constants::kBoolTypeName);
  EXPECT_EQ(TypeName(variant_t(42)), constants::kIntTypeName);
  EXPECT_EQ(TypeName(variant_t(42.4)), constants::kDoubleTypeName);
  EXPECT_EQ(TypeName(variant_t(std::string("abc"))), constants::kStringTypeName);
  EXPECT_EQ(TypeName(variant_t(std::vector<double>({1.0, 1.1, 1.2}))),
            constants::kVectorDoubleTypeName);
  EXPECT_EQ(TypeName(variant_t(ComboProperty::CreateFrom({"a1"}))),
            constants::kComboPropertyTypeName);
  EXPECT_EQ(TypeName(variant_t(ExternalProperty("text", "red"))),
            constants::kExternalPropertyTypeName);
}

TEST_F(VariantTest, DataRoleComparison)
{
  datarole_t data_role1{42, 0};
  datarole_t data_role2{42, 0};
  EXPECT_TRUE(data_role1 == data_role2);

  datarole_t data_role3{std::vector<double>{1, 2, 3}, 42};
  datarole_t data_role4{std::vector<double>{1, 2, 3}, 42};
  EXPECT_TRUE(data_role3 == data_role4);

  datarole_t data_role5{std::vector<double>{1, 2}, 42};
  datarole_t data_role6{std::vector<double>{1, 2, 3}, 42};
  EXPECT_FALSE(data_role5 == data_role6);
}
