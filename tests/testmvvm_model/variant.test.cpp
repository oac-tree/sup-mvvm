// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/core/variant.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing variant_t and utility functions.

class VariantTest : public ::testing::Test
{
};

//! Testing Utils::IsValid function.

TEST_F(VariantTest, IsValid)
{
  using Utils::IsValid;
  EXPECT_FALSE(IsValid(variant_t()));
  EXPECT_TRUE(IsValid(variant_t(true)));
  EXPECT_TRUE(IsValid(variant_t(42)));
  EXPECT_TRUE(IsValid(variant_t(42.1)));
  EXPECT_TRUE(IsValid(variant_t(std::string("abc"))));
  EXPECT_TRUE(IsValid(variant_t(std::vector<double>({1.0, 1.1, 1.2}))));
}

//! Testing Utils::AreCompatible function.

TEST_F(VariantTest, AreCompatible)
{
  std::vector<variant_t> variants = {variant_t(true), variant_t(42), variant_t(42.1),
                                     variant_t(std::string("abc")),
                                     variant_t(std::vector<double>({1.1, 2.2, 3.3}))};
  for (size_t i = 0; i < variants.size(); ++i)
  {
    EXPECT_TRUE(Utils::AreCompatible(variant_t(), variants[i]));
    for (size_t j = 0; j < variants.size(); ++j)
    {
      if (i == j)
      {
        EXPECT_TRUE(Utils::AreCompatible(variants[i], variants[j]));
      }
      else
      {
        EXPECT_FALSE(Utils::AreCompatible(variants[i], variants[j]));
      }
    }
  }
}

TEST_F(VariantTest, TypeName)
{
  using Utils::TypeName;
  EXPECT_EQ(TypeName(variant_t()), Constants::kUndefinedTypeName);
  EXPECT_EQ(TypeName(variant_t(true)), Constants::kBoolTypeName);
  EXPECT_EQ(TypeName(variant_t(42)), Constants::kIntTypeName);
  EXPECT_EQ(TypeName(variant_t(42.4)), Constants::kDoubleTypeName);
  EXPECT_EQ(TypeName(variant_t(std::string("abc"))), Constants::kStringTypeName);
  EXPECT_EQ(TypeName(variant_t(std::vector<double>({1.0, 1.1, 1.2}))),
            Constants::kVectorDoubleTypeName);
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
