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

#include "custom_plot_test_utils.h"
#include "mvvm/plotting/customplot/graph_viewport_plot_controller.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

using namespace mvvm;

//! Testing GraphViewportPlotController.

class GraphViewportPlotControllerTest : public ::testing::Test
{
};

//! Initial state.
TEST_F(GraphViewportPlotControllerTest, InitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

//! Check ::SetItem() method when no graphs exist.
TEST_F(GraphViewportPlotControllerTest, SetItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(item);

  // no graphs in empty GraphViewportItem
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // axis should be [0, 1] as in defaule ViewportAxisItem
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().lower, 0.0);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().upper, 1.0);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().lower, 0.0);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().upper, 1.0);
}

//! Check ::setItem() method when ViewPortItem contains graphs.
// FYI test was failing on Qt 6.4.0 (Nan in ticks parameters of
// CPAxisPainterPrivate::placeTickLabel)
TEST_F(GraphViewportPlotControllerTest, AddGraphAndSetItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

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
  controller.SetItem(viewport_item);

  // single graph on custom plot.
  EXPECT_EQ(custom_plot->graphCount(), 1);

  // QCustomPlot axis should correspond to
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().lower, expected_centers[0]);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().upper, expected_centers[2]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().lower, expected_values[0]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().upper, expected_values[2]);
}

//! Checks consequitive graph adding/removal
TEST_F(GraphViewportPlotControllerTest, AddAndRemoveGraphs)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(viewport_item);

  // No graphs yet.
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // Populating with data items
  auto data1 = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values1 = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data1->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data1->SetValues(expected_values1);

  auto data2 = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values2 = {4.0, 5.0, 6.0};
  data2->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data2->SetValues(expected_values2);

  // adding graph item to viewport
  auto graph_item1 = model.InsertItem<GraphItem>(viewport_item, TagIndex::Default(0));

  // check that QCustomPlot knows about graph
  EXPECT_EQ(custom_plot->graphCount(), 1);

  graph_item1->SetDataItem(data1);

  // check that QCustomPlot knows about graph
  EXPECT_EQ(custom_plot->graphCount(), 1);

  // adding secong graph
  auto graph_item2 = model.InsertItem<GraphItem>(viewport_item, TagIndex::Default(1));
  graph_item2->SetDataItem(data2);

  // check that QCustomPlot knows about two graph
  EXPECT_EQ(custom_plot->graphCount(), 2);

  // Checking that viewport min, max adjusted to both graphs when manually call update_viewport()
  viewport_item->SetViewportToContent();
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().lower, expected_centers[0]);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().upper, expected_centers[2]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().lower, expected_values1[0]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().upper, expected_values2[2]);

  // removing one GraphItem
  model.RemoveItem(graph_item2);

  // only single graph should remain on QCustomPlot3
  EXPECT_EQ(custom_plot->graphCount(), 1);
}

//! Checks consequitive graph adding/removal
TEST_F(GraphViewportPlotControllerTest, AddMoreGraphs)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(viewport_item);

  // No graphs yet.
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // adding graph item to viewport
  model.InsertItem<GraphItem>(viewport_item);
  EXPECT_EQ(custom_plot->graphCount(), 1);

  model.InsertItem<GraphItem>(viewport_item);
  EXPECT_EQ(custom_plot->graphCount(), 2);

  model.InsertItem<GraphItem>(viewport_item);
  EXPECT_EQ(custom_plot->graphCount(), 3);
}

//! Checks The fucntionality of selection in the viewport
TEST_F(GraphViewportPlotControllerTest, CheckVisible)
{
  // Convenience
  struct FindVisible
  {
    static std::vector<QCPAbstractPlottable*> findVisible(const QCustomPlot* custom_plot)
    {
      std::vector<QCPAbstractPlottable*> output;
      for (int i = 0; i < custom_plot->graphCount(); ++i)
      {
        if (custom_plot->graph(i)->visible())
        {
          output.push_back(custom_plot->graph(i));
        }
      }
      return output;
    }
  };

  // custom plot setup
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(viewport_item);

  // adding graph item to viewport
  auto first_plot = model.InsertItem<GraphItem>(viewport_item);
  auto second_plot = model.InsertItem<GraphItem>(viewport_item);
  auto third_plot = model.InsertItem<GraphItem>(viewport_item);
  EXPECT_EQ(custom_plot->graphCount(), 3);

  viewport_item->SetVisible(std::vector<GraphItem*>{first_plot});
  EXPECT_EQ(FindVisible::findVisible(custom_plot.get()).size(), 1);

  viewport_item->SetVisible(std::vector<GraphItem*>{second_plot, third_plot});
  EXPECT_EQ(FindVisible::findVisible(custom_plot.get()).size(), 2);

  viewport_item->SetAllVisible();
  EXPECT_EQ(FindVisible::findVisible(custom_plot.get()).size(), 3);
}

//! Two GraphViewportItem's and switch between them.
TEST_F(GraphViewportPlotControllerTest, SwitchBetweenTwoViewports)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item0 = model.InsertItem<GraphViewportItem>();
  auto viewport_item1 = model.InsertItem<GraphViewportItem>();

  auto data_item = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  auto graph_item = model.InsertItem<GraphItem>(viewport_item0);
  graph_item->SetDataItem(data_item);
  controller.SetItem(viewport_item0);

  // single graph on custom plot.
  EXPECT_EQ(custom_plot->graphCount(), 1);

  // switch to second (empty) viewport, QCustomPlot should have no graphs
  controller.SetItem(viewport_item1);
  EXPECT_EQ(custom_plot->graphCount(), 0);
}

//! Adding graph, then undo, then redo again.
TEST_F(GraphViewportPlotControllerTest, addGraphUndoRedo)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;

  auto viewport_item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(viewport_item);

  // No graphs yet.
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // Populating with data items
  auto data1 = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data1->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data1->SetValues(expected_values);

  // beginning of our undo/redo story
  model.SetUndoEnabled(true);
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 0);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 0);

  // adding graph item to viewport
  auto graph_item = model.InsertItem<GraphItem>(viewport_item, {"", 0});
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 1);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 1);

  // assigning data to graph
  graph_item->SetDataItem(data1);
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 2);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 2);

  // validating graph in custom plot
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(testutils::GetBinCenters(custom_plot->graph()), expected_centers);
  EXPECT_EQ(testutils::GetValues(custom_plot->graph()), expected_values);

  // undoing data assignment
  model.GetCommandStack()->Undo();
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 1);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 2);

  // graph is still there, but empty
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(testutils::GetBinCenters(custom_plot->graph()), std::vector<double>());
  EXPECT_EQ(testutils::GetValues(custom_plot->graph()), std::vector<double>());
  EXPECT_EQ(graph_item->GetDataItem(), nullptr);

  // redoing data assignment
  model.GetCommandStack()->Redo();
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 2);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 2);

  // graph is complete again
  EXPECT_EQ(graph_item->GetDataItem(), data1);
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(testutils::GetBinCenters(custom_plot->graph()), expected_centers);
  EXPECT_EQ(testutils::GetValues(custom_plot->graph()), expected_values);
}

//! Adding graph together with data in macro, then undo, then redo again.
TEST_F(GraphViewportPlotControllerTest, addGraphUndoRedoMacro)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;

  auto viewport_item = model.InsertItem<GraphViewportItem>();
  controller.SetItem(viewport_item);

  // No graphs yet.
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // beginning of our undo/redo story
  model.SetUndoEnabled(true);
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 0);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 0);

  // adding data and graph as macro
  model.GetCommandStack()->BeginMacro("addGraph");
  // Populating with data items
  auto data1 = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data1->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data1->SetValues(expected_values);
  auto data_identifier = data1->GetIdentifier();
  // adding graph item to viewport
  auto graph_item = model.InsertItem<GraphItem>(viewport_item, {"", 0});
  // assigning data to graph
  graph_item->SetDataItem(data1);
  model.GetCommandStack()->EndMacro();

  EXPECT_EQ(viewport_item->GetGraphItems()[0]->GetDataItem(),
            utils::GetTopItems<Data1DItem>(&model).at(0));

  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 1);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 1);

  // validating graph in custom plot
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(testutils::GetBinCenters(custom_plot->graph()), expected_centers);
  EXPECT_EQ(testutils::GetValues(custom_plot->graph()), expected_values);

  // undoing macro
  model.GetCommandStack()->Undo();
  EXPECT_EQ(model.GetCommandStack()->GetIndex(), 0);
  EXPECT_EQ(model.GetCommandStack()->GetCommandCount(), 1);

  // no graph and no items
  EXPECT_EQ(viewport_item->GetGraphItems().size(), 0);
  EXPECT_EQ(utils::GetTopItems<Data1DItem>(&model).size(), 0);
  EXPECT_EQ(custom_plot->graphCount(), 0);

  // redoing macro
  model.GetCommandStack()->Redo();
  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_EQ(viewport_item->GetGraphItems().size(), 1);

  EXPECT_EQ(utils::GetTopItems<Data1DItem>(&model).at(0)->GetIdentifier(), data_identifier);
  EXPECT_EQ(viewport_item->GetGraphItems()[0]->GetDataItem(),
            utils::GetTopItems<Data1DItem>(&model).at(0));

  EXPECT_EQ(testutils::GetBinCenters(custom_plot->graph()), expected_centers);
  EXPECT_EQ(testutils::GetValues(custom_plot->graph()), expected_values);
}
