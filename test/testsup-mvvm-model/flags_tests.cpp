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
    NoOptions,
    kLength,
    kHeight,
    kWidth,
    size
  };
};

TEST_F(FlagsTest, InitilaState)
{
  const Flags<Option> flags;

  EXPECT_EQ(flags.count(), 0);
  EXPECT_EQ(flags.size(), static_cast<std::size_t>(Option::size));
}

TEST_F(FlagsTest, BasicUsage)
{

}


}  // namespace mvvm::test
