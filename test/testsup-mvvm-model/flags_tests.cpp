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

#include "mvvm/core/flags.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for Flags class.
 */
class FlagsTest : public ::testing::Test
{
public:
  enum class Option : std::uint8_t
  {
    kLength,
    kHeight,
    kWidth,
    size
  };
};

TEST_F(FlagsTest, InitilaState)
{
  const Flags<Option> flags;

  EXPECT_EQ(flags.count(), 0U);
  EXPECT_EQ(flags.size(), static_cast<std::size_t>(Option::size));
  EXPECT_TRUE(flags.none());
  EXPECT_FALSE(flags.any());
  EXPECT_FALSE(flags.all());
  EXPECT_EQ(flags.to_ulong(), 0U);
}

TEST_F(FlagsTest, SetUnsetSingleFlag)
{
  Flags<Option> flags;

  flags.set(Option::kLength, true);
  EXPECT_EQ(flags.count(), 1U);
  EXPECT_FALSE(flags.none());
  EXPECT_TRUE(flags.any());
  EXPECT_FALSE(flags.all());
  EXPECT_EQ(flags.to_ulong(), 1U);

  flags.reset();
  EXPECT_EQ(flags.count(), 0U);
  EXPECT_TRUE(flags.none());
  EXPECT_FALSE(flags.any());
  EXPECT_FALSE(flags.all());
  EXPECT_EQ(flags.to_ulong(), 0U);
}

}  // namespace mvvm::test
