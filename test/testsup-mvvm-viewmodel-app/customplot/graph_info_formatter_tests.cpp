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

#include "mvvm/plotting/customplot/graph_info_formatter.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/plotting/customplot/graph_viewport_plot_controller.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QSignalSpy>

namespace mvvm::test
{

/**
 * @brief Tests for GraphInfoFormatter class.
 */
class GraphInfoFormatterTest : public ::testing::Test
{
};

TEST_F(GraphInfoFormatterTest, GetContextString)
{
  {
    const CanvasGraphContext context{1.0, 2.0, false};
    EXPECT_EQ(GetContextString(context), std::string("[x: 1.0, y: 2.0] "));
  }

  {
    const CanvasGraphContext context{1.0, 2.0, true, 100, 42.0};
    EXPECT_EQ(GetContextString(context),
              std::string("[x: 1.0, y: 2.0] [binx: 100] [value: 4.2e+01]"));
  }
}

TEST_F(GraphInfoFormatterTest, SetGraphItemAndCheckPlottables)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController viewport_controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();

  auto data_item = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  auto graph_item = model.InsertItem<GraphItem>(viewport_item);
  graph_item->SetDataItem(data_item);
  viewport_controller.SetItem(viewport_item);

  // single graph on custom plot.
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().lower, expected_centers[0]);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().upper, expected_centers[2]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().lower, expected_values[0]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().upper, expected_values[2]);

  auto graph = custom_plot->graph();
  const double mouse_xpos = 1.5;
  const double mouse_ypos = 2.0;
  EXPECT_EQ(FindGraphNearby(custom_plot.get(), mouse_xpos, mouse_ypos), graph);

  const GraphInfoFormatter formatter;

  const std::string status_string =
      formatter.status_string(custom_plot.get(), mouse_xpos, mouse_ypos);
  EXPECT_EQ(status_string, std::string("[x: 1.5, y: 2.0] [binx: 1] [value: 2.0e+00]"));

  const std::string status_string2 =
      formatter.status_string(custom_plot.get(), 100.0, 200.0);
  EXPECT_EQ(status_string2, std::string("[x: 100.0, y: 200.0] "));
}

}  // namespace mvvm::test
