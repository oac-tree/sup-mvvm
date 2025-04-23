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

#include "mvvm/standarditems/waveform_helper.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for helper methods from waveform_helper.h

class VectorItemTests : public ::testing::Test
{
};

TEST_F(VectorItemTests, GetPairOfVectors)
{
  waveform_data_t waveform_data = {{1.0, 10.0}, {2.0, 20.0}};

  auto [x_vec, y_vec] = GetPairOfVectors(waveform_data);

  EXPECT_EQ(x_vec, std::vector<double>({1.0, 2.0}));
  EXPECT_EQ(y_vec, std::vector<double>({10.0, 20.0}));
}
