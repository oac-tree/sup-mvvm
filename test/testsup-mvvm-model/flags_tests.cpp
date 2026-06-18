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

  EXPECT_EQ(flags.Count(), 0U);
  EXPECT_EQ(flags.GetSize(), static_cast<std::size_t>(Option::size));
  EXPECT_TRUE(flags.None());
  EXPECT_FALSE(flags.Any());
  EXPECT_FALSE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 0U);
  EXPECT_FALSE(flags[Option::kLength]);
  EXPECT_FALSE(flags[Option::kHeight]);
  EXPECT_FALSE(flags[Option::kWidth]);
  EXPECT_THROW(flags[Option::size], RuntimeException);

  EXPECT_FALSE(flags.IsSet(Option::kLength));
  EXPECT_FALSE(flags.IsSet(Option::kHeight));
  EXPECT_FALSE(flags.IsSet(Option::kWidth));
  EXPECT_THROW(flags.IsSet(Option::size), RuntimeException);
}

TEST_F(FlagsTest, SetUnsetSingleFlag)
{
  Flags<Option> flags;

  EXPECT_EQ(flags.GetSize(), static_cast<std::size_t>(Option::size));

  flags.Set(Option::kLength, true);
  EXPECT_EQ(flags.Count(), 1U);
  EXPECT_FALSE(flags.None());
  EXPECT_TRUE(flags.Any());
  EXPECT_FALSE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 1U);

  EXPECT_TRUE(flags[Option::kLength]);
  EXPECT_FALSE(flags[Option::kHeight]);
  EXPECT_FALSE(flags[Option::kWidth]);

  flags.Reset(Option::kLength);
  EXPECT_EQ(flags.Count(), 0U);
  EXPECT_TRUE(flags.None());
  EXPECT_FALSE(flags.Any());
  EXPECT_FALSE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 0U);

  EXPECT_THROW(flags.Set(Option::size, true), RuntimeException);
}

TEST_F(FlagsTest, SetTwoFlags)
{
  Flags<Option> flags;

  flags.Set(Option::kLength, true);
  flags.Set(Option::kHeight, true);
  EXPECT_EQ(flags.Count(), 2U);
  EXPECT_FALSE(flags.None());
  EXPECT_TRUE(flags.Any());
  EXPECT_FALSE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 3U);

  EXPECT_TRUE(flags[Option::kLength]);
  EXPECT_TRUE(flags[Option::kHeight]);
  EXPECT_FALSE(flags[Option::kWidth]);

  EXPECT_TRUE(flags.IsSet(Option::kLength));
  EXPECT_TRUE(flags.IsSet(Option::kHeight));
  EXPECT_FALSE(flags.IsSet(Option::kWidth));

  flags.Reset();
  EXPECT_EQ(flags.Count(), 0U);
  EXPECT_TRUE(flags.None());
  EXPECT_FALSE(flags.Any());
  EXPECT_FALSE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 0U);
}

TEST_F(FlagsTest, ToggleFlag)
{
  Flags<Option> flags;

  flags.Toggle(Option::kLength);
  EXPECT_EQ(flags.Count(), 1U);
  EXPECT_TRUE(flags[Option::kLength]);

  flags.Toggle(Option::kLength);
  EXPECT_EQ(flags.Count(), 0U);
  EXPECT_FALSE(flags[Option::kLength]);
}

TEST_F(FlagsTest, SetAllFlags)
{
  Flags<Option> flags;

  flags.Set(Option::kLength, true);
  flags.Set(Option::kHeight, true);
  flags.Set(Option::kWidth, true);
  EXPECT_EQ(flags.Count(), 3U);
  EXPECT_FALSE(flags.None());
  EXPECT_TRUE(flags.Any());
  EXPECT_TRUE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 7U);
}

TEST_F(FlagsTest, ConstructFromValue)
{
  const Flags<Option> flags(7U);

  EXPECT_EQ(flags.Count(), 3U);
  EXPECT_FALSE(flags.None());
  EXPECT_TRUE(flags.Any());
  EXPECT_TRUE(flags.All());
  EXPECT_EQ(flags.to_ulong(), 7U);
}

TEST_F(FlagsTest, ConstructFromVector)
{
  const Flags<Option> flags({Option::kLength, Option::kWidth});

  EXPECT_TRUE(flags.IsSet(Option::kLength));
  EXPECT_FALSE(flags.IsSet(Option::kHeight));
  EXPECT_TRUE(flags.IsSet(Option::kWidth));
}

TEST_F(FlagsTest, EqualityOperators)
{
  const Flags<Option> flags1({Option::kLength, Option::kWidth});
  const Flags<Option> flags2({Option::kLength, Option::kWidth});
  const Flags<Option> flags3({Option::kLength, Option::kHeight});
  EXPECT_TRUE(flags1 == flags2);
  EXPECT_FALSE(flags1 != flags2);
  EXPECT_FALSE(flags1 == flags3);
  EXPECT_TRUE(flags1 != flags3);
}

}  // namespace mvvm::test
