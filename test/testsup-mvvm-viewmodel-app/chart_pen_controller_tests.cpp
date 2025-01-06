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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/charts/chart_pen_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>

using namespace mvvm;

class ChartPenControllerTest : public ::testing::Test
{
};

//! Behavior of the controller when no PenItem is set.

TEST_F(ChartPenControllerTest, InitialState)
{
  EXPECT_THROW(ChartPenController(nullptr), std::runtime_error);

  QLineSeries line_series;
  // checking what QLineSeries has as a default pen
  const auto pen1 = line_series.pen();
  EXPECT_EQ(pen1.color(), QColor(Qt::black));
  EXPECT_EQ(pen1.style(), Qt::SolidLine);
  EXPECT_EQ(pen1.width(), 1);

  const ChartPenController controller(&line_series);

  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_EQ(controller.GetQtLineSeries(), &line_series);

  // everything stays the same after controller is in charge, since there is no item set
  const auto pen2 = line_series.pen();
  EXPECT_EQ(pen2.color(), QColor(Qt::black));
  EXPECT_EQ(pen2.style(), Qt::SolidLine);
  EXPECT_EQ(pen2.width(), 1);
}

//! Studying Qt: after adding QLineSeries to the QChart, the latter changes the pen of just added
//! line series. What is this pen?

TEST_F(ChartPenControllerTest, DefaultQtLineSeriesPen)
{
  QChart chart;

  auto line_series = std::make_unique<QLineSeries>();
  auto line_series_ptr = line_series.get();

  line_series->append({1.0, 2.0});
  chart.addSeries(line_series.release());  // ownership is taken by Qt

  // this is what QChart assign as a pen to the first LineSeries
  const auto pen = line_series_ptr->pen();
  EXPECT_EQ(pen.color(), QColor("#209fdf"));
  EXPECT_EQ(pen.style(), Qt::SolidLine);
  EXPECT_EQ(pen.width(), 2);

  // Just for the record. Qt assigns to the 3 first line series following colors: "#209fdf",
  // "#99ca53" and "#f6a625", i.e. sort of the blue, green and orange.
}

//! Setting item to the controller and checking the pen of line series after first setup.

TEST_F(ChartPenControllerTest, GraphItemInInitialState)
{
  QLineSeries line_series;

  ChartPenController controller(&line_series);

  mvvm::ApplicationModel model;
  auto pen_item = model.InsertItem<mvvm::PenItem>();
  controller.SetItem(pen_item);

  EXPECT_EQ(controller.GetItem(), pen_item);

  // parameters of pen should coincide to mvvm::PenItem initial settings
  const auto pen = line_series.pen();
  EXPECT_EQ(pen.color(), QColor(Qt::black));
  EXPECT_EQ(pen.style(), Qt::SolidLine);
  EXPECT_EQ(pen.width(), 1);
}

//! Setting item to the controller and checking the pen of line series after first setup.
//! Same as previous test, except that line series already belongs to the chart.

TEST_F(ChartPenControllerTest, GraphItemInChartInitialState)
{
  QChart chart;

  auto line_series = std::make_unique<QLineSeries>();
  auto line_series_ptr = line_series.get();

  line_series->append({1.0, 2.0});
  chart.addSeries(line_series.release());  // ownership is taken by Qt

  ChartPenController controller(line_series_ptr);

  mvvm::ApplicationModel model;
  auto pen_item = model.InsertItem<mvvm::PenItem>();
  controller.SetItem(pen_item);

  EXPECT_EQ(controller.GetItem(), pen_item);

  // parameters of pen should coincide to mvvm::PenItem initial settings
  const auto pen = line_series_ptr->pen();
  EXPECT_EQ(pen.color(), QColor(Qt::black));
  EXPECT_EQ(pen.style(), Qt::SolidLine);
  EXPECT_EQ(pen.width(), 1);
}

//! Changing pen through item and validating QLineSeries.

TEST_F(ChartPenControllerTest, SetColorAndWidth)
{
  QChart chart;

  auto line_series = std::make_unique<QLineSeries>();
  auto line_series_ptr = line_series.get();

  line_series->append({1.0, 2.0});
  chart.addSeries(line_series.release());  // ownership is taken by Qt

  ChartPenController controller(line_series_ptr);

  mvvm::ApplicationModel model;
  auto pen_item = model.InsertItem<mvvm::PenItem>();
  controller.SetItem(pen_item);

  pen_item->SetWidth(3);
  pen_item->SetNamedColor("red");

  // parameters of line series
  const auto pen = line_series_ptr->pen();
  EXPECT_EQ(pen.color(), QColor(Qt::red));
  EXPECT_EQ(pen.style(), Qt::SolidLine);
  EXPECT_EQ(pen.width(), 3);

  // set color via named color machinery
  // see https://www.w3.org/TR/css-color-3/#svg-color
  pen_item->SetNamedColor("azure");
  EXPECT_EQ(line_series_ptr->pen().color().name(), QString("#f0ffff"));
}
