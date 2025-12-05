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

#include "mvvm/views/chart_view.h"

#include <mvvm/plotting/charts/qt_charts.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class ChartViewTest : public ::testing::Test
{
};

TEST_F(ChartViewTest, InitialState)
{
  auto chart = std::make_unique<QChart>();
  auto chart_ptr = chart.get();
  const auto initial_options = chart_ptr->animationOptions();

  const ChartView chart_view(chart.release());

  // animations options are not chainged
  EXPECT_EQ(chart_ptr->animationOptions(), initial_options);
}

}  // namespace mvvm::test
