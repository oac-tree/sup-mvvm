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

#include "mvvm/viewmodel/customvariants.h"

#include "test_utils.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests for utilities related to custom

class CustomVariantTest : public ::testing::Test
{
};

//! Checks variants against IsBoolVariant, IsIntVariant, etc functions.

TEST_F(CustomVariantTest, IsVariantType)
{
  using is_variant_t = std::function<bool(const QVariant&)>;

  std::vector<std::pair<QVariant, is_variant_t>> data = {
      {QVariant::fromValue(true), Utils::IsBoolVariant},
      {QVariant::fromValue(1), Utils::IsIntVariant},
      {QVariant::fromValue(42.0), Utils::IsDoubleVariant},
      {QVariant::fromValue(std::string("string1")), Utils::IsStdStringVariant},
      {QVariant::fromValue(std::vector<double>({1, 2})), Utils::IsDoubleVectorVariant}};

  for (size_t i = 0; i < data.size(); ++i)
  {
    auto is_variant_func = data[i].second;
    for (size_t j = 0; j < data.size(); ++j)
    {
      auto variant = data[j].first;
      if (i == j)
        EXPECT_TRUE(is_variant_func(variant));
      else
        EXPECT_FALSE(is_variant_func(variant));
    }
  }
}
