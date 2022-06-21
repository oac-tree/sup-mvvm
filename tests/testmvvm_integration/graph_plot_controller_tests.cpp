/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/graph_plot_controller.h"

#include "custom_plot_test_utils.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QSignalSpy>

using namespace mvvm;

//! Testing GraphPlotController.

class GraphPlotControllerTests : public ::testing::Test
{
};

//! Initial state.

TEST_F(GraphPlotControllerTests, initialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());
  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

//! Setting GraphItem with data and checking that plottable contains correct data.

TEST_F(GraphPlotControllerTests, setItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2, 0.0, 2.0);
  std::vector<double> expected_centers = {0.5, 1.5};
  std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  // setup graph item
  auto graph_item = model.InsertItem<GraphItem>();
  graph_item->SetDataItem(data_item);

  // initializing controller
  controller.SetItem(graph_item);

  // Checking resulting plottables
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(testutils::GetBinCenters(graph), expected_centers);
  EXPECT_EQ(testutils::GetValues(graph), expected_values);
  EXPECT_EQ(graph->pen().color(), QColor(Qt::black));
  EXPECT_EQ(graph->pen().style(), Qt::SolidLine);
  EXPECT_EQ(graph->pen().width(), 1);
}

TEST_F(GraphPlotControllerTests, changeGraphAppearance)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2, 0.0, 2.0);
  std::vector<double> expected_centers = {0.5, 1.5};
  std::vector<double> expected_values = {42.0, 43.0};
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

//! Setting GraphItem with data and checking that plottable contains correct data.
//! Same as above, except that the data is based on PointWiseAxis.

TEST_F(GraphPlotControllerTests, setPointwiseItem)
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
  EXPECT_EQ(testutils::GetBinCenters(graph), expected_centers);
  EXPECT_EQ(testutils::GetValues(graph), expected_values);
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));
}

//! Setting data to graph after.

TEST_F(GraphPlotControllerTests, SetDataAfter)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  ApplicationModel model;
  auto graph_item = model.InsertItem<GraphItem>();

  controller.SetItem(graph_item);

  // without data QCustomPlot has a graph without points
  EXPECT_EQ(custom_plot->graphCount(), 1);
  auto graph = custom_plot->graph();
  EXPECT_EQ(testutils::GetBinCenters(graph), std::vector<double>());
  EXPECT_EQ(testutils::GetValues(graph), std::vector<double>());

  // setup Data1DItem and assign to GraphItem
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2, 0.0, 2.0);
  std::vector<double> expected_centers = {0.5, 1.5};
  std::vector<double> expected_values = {42.0, 43.0};
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);

  // Checking resulting plottables
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(testutils::GetBinCenters(graph), expected_centers);
  EXPECT_EQ(testutils::GetValues(graph), expected_values);
}

//! Unlinking from Data1DItem or GraphItem.

TEST_F(GraphPlotControllerTests, UnlinkFromItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphPlotController controller(custom_plot.get());

  // setup model and single data item in it
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2, 0.0, 2.0);
  std::vector<double> expected_centers = {0.5, 1.5};
  std::vector<double> expected_values = {42.0, 43.0};
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
  EXPECT_EQ(testutils::GetBinCenters(graph), std::vector<double>());
  EXPECT_EQ(testutils::GetValues(graph), std::vector<double>());
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));

  // unlinking from graph item should remove Graph from CustomPlot
  controller.SetItem(nullptr);
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

//! Deletion of controller should lead to graph removal.

TEST_F(GraphPlotControllerTests, controllerDelete)
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

//! Deletion of graphItem should lead to the dissapearance of graph.
//!
//! FIXME restore test after implementation of signal on item destruction

// TEST_F(GraphPlotControllerTests, graphDelete)
//{
//   auto custom_plot = std::make_unique<QCustomPlot>();
//   auto controller = std::make_unique<GraphPlotController>(custom_plot.get());

//  // setup model and single data item in it
//  ApplicationModel model;
//  auto data_item = model.InsertItem<Data1DItem>();

//  // setup graph item
//  auto graph_item = model.InsertItem<GraphItem>();
//  graph_item->SetDataItem(data_item);

//  // initializing controller
//  controller->SetItem(graph_item);
//  EXPECT_EQ(custom_plot->graphCount(), 1);

//  model.RemoveItem(graph_item);
//  EXPECT_EQ(custom_plot->graphCount(), 0);
//}
