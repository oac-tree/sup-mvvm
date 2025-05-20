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

#include "mvvm/core/variant_index.h"

#include <mvvm/signals/event_types.h>

#include <gtest/gtest.h>

#include <string>

using namespace mvvm;

/**
 * @brief Tests for helper functions from variant_index_h
 */
class VariantIndexTests : public ::testing::Test
{
};

TEST_F(VariantIndexTests, VariantIndex)
{
  {
    const event_variant_t event_variant{DataChangedEvent{}};
    const auto index = variant_index<event_variant_t, DataChangedEvent>();
    EXPECT_EQ(index, event_variant.index());
  }

  {
    const event_variant_t event_variant{ModelAboutToBeDestroyedEvent{}};
    const auto index = variant_index<event_variant_t, ModelAboutToBeDestroyedEvent>();
    EXPECT_EQ(index, event_variant.index());
  }

  {  // type not present in variant
    const auto index = variant_index<event_variant_t, std::string>();
    EXPECT_EQ(index, std::variant_size_v<event_variant_t>);
  }
}

