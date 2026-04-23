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

#include "custom_plot_test_utils.h"
#include "mvvm/plotting/customplot/graph_plot_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

namespace mvvm::test
{

/**
 * @brief Tests for GraphPlotController class.
 */
class GraphPlotControllerTest : public ::testing::Test
{
};

TEST_F(GraphPlotControllerTest, InitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  const GraphPlotController controller(custom_plot.get());
  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

TEST_F(GraphPlotControllerTest, SetGraphItemAndCheckPlottables)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2U, 0.0, 2.0);
  const std::vector<double> expected_centers = {0.5, 1.5};
  const std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller.SetItem(graph_item);

  // Checking resulting plottables
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(GetBinCenters(graph), expected_centers);
  EXPECT_EQ(GetValues(graph), expected_values);
  EXPECT_EQ(graph->pen().color(), QColor(Qt::black));
  EXPECT_EQ(graph->pen().style(), Qt::SolidLine);
  EXPECT_EQ(graph->pen().width(), 1);
}

TEST_F(GraphPlotControllerTest, ChangeGraphAppearance)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2U, 0.0, 2.0);
  const std::vector<double> expected_centers = {0.5, 1.5};
  const std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller.SetItem(graph_item);

  // changing appearance properties
  auto pen_item = graph_item->GetPenItem();
  pen_item->SetNamedColor("red");

  auto styleCombo = pen_item->Property<ComboProperty>(PenItem::kStyle);
  styleCombo.SetCurrentIndex(2);
  pen_item->SetProperty(PenItem::kStyle, styleCombo);
  pen_item->SetWidth(2);

  auto graph = custom_plot->graph();
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));
  EXPECT_EQ(graph->pen().style(), Qt::DashLine);
  EXPECT_EQ(graph->pen().width(), 2);
}

TEST_F(GraphPlotControllerTest, ControllerOnPointWiseAxis)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  const std::vector<double> expected_centers = {1.0, 2.0, 3.0};
  const std::vector<double> expected_values = {42.0, 43.0, 44.0};

  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<PointwiseAxisItem>(expected_centers);
  data_item->SetValues(expected_values);

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  auto pen_item = graph_item->GetPenItem();
  pen_item->SetNamedColor("red");
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller.SetItem(graph_item);

  // Checking resulting plottables
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(GetBinCenters(graph), expected_centers);
  EXPECT_EQ(GetValues(graph), expected_values);
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));
}

TEST_F(GraphPlotControllerTest, SetDataToGraphAfter)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  ApplicationModel model;
  auto graph_item = model.InsertItem<GraphItem>();

  controller.SetItem(graph_item);

  // without data QCustomPlot has a graph without points
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(GetBinCenters(graph), std::vector<double>());
  EXPECT_EQ(GetValues(graph), std::vector<double>());

  // setup Data1DItem and assign to GraphItem
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2U, 0.0, 2.0);
  const std::vector<double> expected_centers = {0.5, 1.5};
  const std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);

  // Checking resulting plottables
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(GetBinCenters(graph), expected_centers);
  EXPECT_EQ(GetValues(graph), expected_values);
}

TEST_F(GraphPlotControllerTest, UnlinkFromItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2U, 0.0, 2.0);
  const std::vector<double> expected_centers = {0.5, 1.5};
  const std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  auto pen_item = graph_item->GetPenItem();
  pen_item->SetNamedColor("red");
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller.SetItem(graph_item);

  // unlinking from data item
  graph_item->SetDataItem(nullptr);

  // Checking resulting plottables
  // Current convention is that graph stays intact, but points disappear.
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(GetBinCenters(graph), std::vector<double>());
  EXPECT_EQ(GetValues(graph), std::vector<double>());
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));

  // unlinking from graph item should remove Graph from CustomPlot
  controller.SetItem(nullptr);
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

TEST_F(GraphPlotControllerTest, ControllerDeleteLeadToGraphRemoval)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto controller = std::make_unique<GraphPlotController>(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller->SetItem(graph_item);
  EXPECT_EQ(custom_plot->graphCount(), 1);

  // deleting controller should lead to graph removal
  controller.reset();
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

}  // namespace mvvm::test
