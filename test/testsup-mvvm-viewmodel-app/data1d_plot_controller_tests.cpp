/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_plot_test_utils.h"

#include <mvvm/model/application_model.h>
#include <mvvm/plotting/customplot/data1d_plot_controller.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

using namespace mvvm;

//! Testing Data1DPlotController.

class Data1DPlotControllerTest : public ::testing::Test
{
};

//! Initial state.

TEST_F(Data1DPlotControllerTest, InitialState)
{
  // Constructor accept valid QCPGraph
  EXPECT_THROW(Data1DPlotController(nullptr), RuntimeException);

  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  Data1DPlotController controller(graph);
  EXPECT_EQ(controller.GetItem(), nullptr);

  // no points have been added to graph
  EXPECT_EQ(std::vector<double>(), testutils::GetBinCenters(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetValues(graph));
}

//! Testing controller when Data1DItem is not initialized properly.

TEST_F(Data1DPlotControllerTest, DataItemInInitialState)
{
  // creating custom plot and empty graph on it
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  // creating data item with single point
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();

  // creating controller and point it to Data1DItem
  Data1DPlotController controller(graph);
  controller.SetItem(data_item);

  EXPECT_EQ(std::vector<double>(), testutils::GetBinCenters(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetValues(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetErrors(graph));
}

//! Testing controller when Data1DItem get it's axis after controller setup.

TEST_F(Data1DPlotControllerTest, AxisAfter)
{
  // creating custom plot and empty graph on it
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  // creating data item with single point
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();

  // creating controller and point it to Data1DItem
  Data1DPlotController controller(graph);
  controller.SetItem(data_item);

  // setting correct axis
  data_item->SetAxis<FixedBinAxisItem>(1, 1.0, 2.0);
  EXPECT_EQ(data_item->GetBinCenters(), testutils::GetBinCenters(graph));
  EXPECT_EQ(data_item->GetValues(), testutils::GetValues(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetErrors(graph));
}

//! Testing graph points update.

TEST_F(Data1DPlotControllerTest, DataPoints)
{
  // creating custom plot and empty graph on it
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  // creating data item with single point
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(1, 1.0, 2.0);

  // creating controller and point it to Data1DItem
  Data1DPlotController controller(graph);
  controller.SetItem(data_item);

  // checking that QCPGraph now has data points as in Data1DItem
  EXPECT_EQ(data_item->GetBinCenters(), testutils::GetBinCenters(graph));
  EXPECT_EQ(data_item->GetValues(), testutils::GetValues(graph));
  EXPECT_EQ(data_item->GetErrors(), testutils::GetErrors(graph));

  // Setting item to nullptr. Current convention is that graph stays intact, but points disappear.
  controller.SetItem(nullptr);
  EXPECT_EQ(std::vector<double>(), testutils::GetBinCenters(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetValues(graph));
  EXPECT_EQ(std::vector<double>(), testutils::GetErrors(graph));
}

//! Testing graph errors update.

TEST_F(Data1DPlotControllerTest, ErrorBars)
{
  // creating custom plot and empty graph on it
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  // creating data item with single point
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  data_item->SetAxis<FixedBinAxisItem>(2, 1.0, 2.0);

  // creating controller and point it to Data1DItem
  Data1DPlotController controller(graph);
  controller.SetItem(data_item);

  std::vector<double> expected_errors = {0.1, 0.2};
  data_item->SetErrors(expected_errors);
  EXPECT_EQ(testutils::GetErrors(graph), expected_errors);

  // setting new errors
  expected_errors = {0.3, 0.4};
  data_item->SetErrors(expected_errors);
  EXPECT_EQ(testutils::GetErrors(graph), expected_errors);
}

//! Testing two graph scenario.

TEST_F(Data1DPlotControllerTest, TwoDataItems)
{
  // creating custom plot and empty graph on it
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  // creating two data items
  ApplicationModel model;
  auto data_item1 = model.InsertItem<Data1DItem>();
  data_item1->SetAxis<FixedBinAxisItem>(1, 1.0, 2.0);
  auto data_item2 = model.InsertItem<Data1DItem>();
  data_item2->SetAxis<FixedBinAxisItem>(2, 0.0, 2.0);

  // creating controller and point it to first item
  Data1DPlotController controller(graph);
  controller.SetItem(data_item1);

  // checking that QCPGraph now has data points as in first data item
  EXPECT_EQ(data_item1->GetBinCenters(), testutils::GetBinCenters(graph));
  EXPECT_EQ(data_item1->GetValues(), testutils::GetValues(graph));

  // pointing controller to the second item
  controller.SetItem(data_item2);
  EXPECT_EQ(data_item2->GetBinCenters(), testutils::GetBinCenters(graph));
  EXPECT_EQ(data_item2->GetValues(), testutils::GetValues(graph));
}
