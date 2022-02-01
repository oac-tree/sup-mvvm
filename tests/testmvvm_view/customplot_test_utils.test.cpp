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

#include "customplot_test_utils.h"

#include <gtest/gtest.h>
#include <qcustomplot.h>

//! Testing "utilr for testing" defined in TestUtils namespace.

class CustomplotTestUtilsTest : public ::testing::Test
{
};

//! Check methods to access graph bin centers and values.

TEST_F(CustomplotTestUtilsTest, binCentersbinValues)
{
  QCustomPlot custom_plot;

  auto graph = custom_plot.addGraph();
  graph->setData(QVector<double>({1, 2, 3}), QVector<double>({10, 20, 30}));

  EXPECT_EQ(TestUtils::GetBinCenters(graph), std::vector<double>({1, 2, 3}));
  EXPECT_EQ(TestUtils::GetValues(graph), std::vector<double>({10, 20, 30}));
}

//! Check methods to access graph errors.

TEST_F(CustomplotTestUtilsTest, binErrors)
{
  QCustomPlot custom_plot;

  auto graph = custom_plot.addGraph();
  graph->setData(QVector<double>({1, 2, 3}), QVector<double>({10, 20, 30}));

  EXPECT_EQ(TestUtils::GetErrors(graph), std::vector<double>());

  auto errorBars = new QCPErrorBars(custom_plot.xAxis, custom_plot.yAxis);
  errorBars->removeFromLegend();
  errorBars->setDataPlottable(graph);
  errorBars->setData(QVector<double>({0.1, 0.2, 0.3}));

  EXPECT_EQ(TestUtils::GetErrors(graph), std::vector<double>({0.1, 0.2, 0.3}));
}
