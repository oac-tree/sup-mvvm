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
  std::vector<variant_t> variants = {variant_t(true),
                                     variant_t(42),
                                     variant_t(42.1),
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
  EXPECT_EQ(TypeName(variant_t()), Constants::undefined_type_name);
  EXPECT_EQ(TypeName(variant_t(true)), Constants::bool_type_name);
  EXPECT_EQ(TypeName(variant_t(42)), Constants::int_type_name);
  EXPECT_EQ(TypeName(variant_t(42.4)), Constants::double_type_name);
  EXPECT_EQ(TypeName(variant_t(std::string("abc"))), Constants::string_type_name);
  EXPECT_EQ(TypeName(variant_t(std::vector<double>({1.0, 1.1, 1.2}))),
            Constants::vector_double_type_name);
}
