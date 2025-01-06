/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/plotting/charts/chart_viewport_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

class ChartViewportControllerTest : public ::testing::Test
{
};

//! Setting empty ChartViewportItem to controller.
TEST_F(ChartViewportControllerTest, SetEmptyViewport)
{
  QChart chart;
  ChartViewportController controller(&chart);

  // initial state: no item to listen, no series
  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_TRUE(chart.series().empty());

  EXPECT_EQ(controller.GetXQtAxis(), nullptr);
  EXPECT_EQ(controller.GetYQtAxis(), nullptr);

  // adding empty viewport item
  mvvm::ApplicationModel model;
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  controller.SetItem(viewport_item);
  EXPECT_EQ(controller.GetItem(), viewport_item);
  EXPECT_TRUE(chart.series().empty());
}

//! Setting initially non-empty ChartViewportItem to controller.
//! It has a single LineSeriesItem without any points.
TEST_F(ChartViewportControllerTest, SetNonEmptyViewport)
{
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // setting viewport with one graph already
  controller.SetItem(viewport_item);

  // checking that QLineSeries object was created, without points
  auto qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);
  auto line_series = dynamic_cast<QLineSeries *>(qt_series.at(0));
  ASSERT_NE(line_series, nullptr);
  EXPECT_EQ(line_series->points().size(), 0);
}

//! Setting initially non-empty ChartViewportItem to controller.
//! It has a single LineSeriesItem without some points on board.
TEST_F(ChartViewportControllerTest, SetViewportWithLineSeries)
{
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;

  // creating viewport with line series
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // setting data to it
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});

  // setting viewport to the controller
  controller.SetItem(viewport_item);

  // checking that QLineSeries object was created, with points properly set
  auto qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);
  auto qt_line_series = dynamic_cast<QLineSeries *>(qt_series.at(0));
  ASSERT_NE(qt_line_series, nullptr);

  auto qt_points = qt_line_series->points();
  EXPECT_EQ(qt_points.size(), 2);

  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);

  // validating axes
  EXPECT_EQ(qt_line_series->attachedAxes(),
            QList<QAbstractAxis *>({controller.GetXQtAxis(), controller.GetYQtAxis()}));
}

//! We setup controller on empty viewport, then add new LineSeriesItem (without points) to it.
//! Corresponding QLineSeries object should appear.
TEST_F(ChartViewportControllerTest, AddLineSeries)
{
  // setting up empty viewport and controller
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  controller.SetItem(viewport_item);

  // adding line series to viewport
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // checking that QLineSeries object was created, without points
  auto qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);
  auto line_series = dynamic_cast<QLineSeries *>(qt_series.at(0));
  ASSERT_NE(line_series, nullptr);
  EXPECT_EQ(line_series->points().size(), 0);

  // validating axes
  EXPECT_EQ(line_series->attachedAxes(),
            QList<QAbstractAxis *>({controller.GetXQtAxis(), controller.GetYQtAxis()}));
}

TEST_F(ChartViewportControllerTest, AddLineSeriesWithPoints)
{
  // setting up empty viewport and controller
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  controller.SetItem(viewport_item);

  // adding line series to viewport
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // checking that we have Qt line series object without points
  auto qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);
  auto qt_line_series = dynamic_cast<QLineSeries *>(qt_series.at(0));
  ASSERT_NE(qt_line_series, nullptr);
  auto qt_points = qt_line_series->points();
  ASSERT_TRUE(qt_points.empty());

  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});

  // checking that QLineSeries object was created, with points properly set
  qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);
  qt_line_series = dynamic_cast<QLineSeries *>(qt_series.at(0));
  ASSERT_NE(qt_line_series, nullptr);

  qt_points = qt_line_series->points();
  ASSERT_EQ(qt_points.size(), 2);

  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
}

TEST_F(ChartViewportControllerTest, RemoveLineSeries)
{
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;

  // creating viewport with line series
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // setting data to it
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});

  // setting viewport to the controller
  controller.SetItem(viewport_item);

  auto qt_series = chart.series();
  ASSERT_EQ(qt_series.size(), 1);

  // removing LineSeriesItem
  model.RemoveItem(line_series_item);

  EXPECT_TRUE(viewport_item->GetLineSeries().empty());

  // checking that Qt's canvas doesn't have plots anymore
  EXPECT_TRUE(chart.series().empty());
}

//! Validating min, max values of viewport axes

TEST_F(ChartViewportControllerTest, AxisControllers)
{
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;

  // creating viewport with line series
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // setting data to it
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {20.0, 200.0}});

  // setting viewport to the controller
  controller.SetItem(viewport_item);

  // expecting some axes created
  auto horizontal_axes = chart.axes(Qt::Horizontal);
  ASSERT_EQ(horizontal_axes.size(), 1);
  auto x_axis = dynamic_cast<QValueAxis *>(horizontal_axes.at(0));
  ASSERT_NE(x_axis, nullptr);

  auto vertical_axes = chart.axes(Qt::Vertical);
  ASSERT_EQ(vertical_axes.size(), 1);
  auto y_axis = dynamic_cast<QValueAxis *>(vertical_axes.at(0));
  ASSERT_NE(y_axis, nullptr);

  // default axes range is (0,1) for both x and y
  EXPECT_EQ(x_axis->min(), 0.0);
  EXPECT_EQ(x_axis->max(), 1.0);
  EXPECT_EQ(y_axis->min(), 0.0);
  EXPECT_EQ(y_axis->max(), 1.0);

  // after setting viewport to content it should fit our data
  viewport_item->SetViewportToContent();
  EXPECT_EQ(x_axis->min(), 1.0);
  EXPECT_EQ(x_axis->max(), 20.0);
  EXPECT_EQ(y_axis->min(), 10.0);
  EXPECT_EQ(y_axis->max(), 200.0);
}

TEST_F(ChartViewportControllerTest, AnimationProperty)
{
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;

  // creating viewport with line series
  auto viewport_item = model.InsertItem<ChartViewportItem>();

  // setting viewport to the controller
  controller.SetItem(viewport_item);

  EXPECT_EQ(chart.animationOptions(), QChart::AllAnimations);

  auto combo = viewport_item->Property<ComboProperty>(ChartViewportItem::kAnimation);

  combo.SetSelected(ChartViewportItem::kGridAnimation, false);
  viewport_item->SetProperty(ChartViewportItem::kAnimation, combo);
  EXPECT_EQ(chart.animationOptions(), QChart::SeriesAnimations);

  combo.SetSelected(ChartViewportItem::kGridAnimation, true);
  combo.SetSelected(ChartViewportItem::kSeriesAnimation, false);
  viewport_item->SetProperty(ChartViewportItem::kAnimation, combo);
  EXPECT_EQ(chart.animationOptions(), QChart::GridAxisAnimations);
}

TEST_F(ChartViewportControllerTest, SHowHideLegend)
{
  // setting up empty viewport and controller
  QChart chart;
  ChartViewportController controller(&chart);

  mvvm::ApplicationModel model;
  auto viewport_item = model.InsertItem<ChartViewportItem>();
  controller.SetItem(viewport_item);

  // adding line series to viewport
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  EXPECT_TRUE(chart.legend()->isVisible());

  viewport_item->SetProperty(ChartViewportItem::kLegendEnabled, false);
  EXPECT_FALSE(chart.legend()->isVisible());
}
