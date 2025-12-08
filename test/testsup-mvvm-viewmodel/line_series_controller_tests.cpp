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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/charts/line_series_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/waveform_helper.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class LineSeriesControllerTest : public ::testing::Test
{
};

TEST_F(LineSeriesControllerTest, InitialState)
{
  EXPECT_THROW(LineSeriesController{nullptr}, RuntimeException);

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  EXPECT_EQ(controller.GetQtLineSeries(), &line_series);
}

//! Checking that points will appear after linking to  the data item.
TEST_F(LineSeriesControllerTest, SetDataItem)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  // initially there is no points on graph
  EXPECT_TRUE(line_series.points().empty());

  // filling the model with line series data: 3 points
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  controller.SetItem(line_series_item);
  EXPECT_EQ(controller.GetItem(), line_series_item);

  // our graph should get points
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

//! Checking that QLineSeries gets proper pen after initialization.
TEST_F(LineSeriesControllerTest, InitialPenStyle)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);

  EXPECT_EQ(controller.GetItem(), line_series_item);

  // filling the model with line series data: 3 points
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  // linking to the data item
  line_series_item->SetDataItem(data_item);

  // validating pen
  const auto pen = line_series.pen();
  EXPECT_EQ(pen.color().name().toStdString(), constants::kDefaultLineColor);
  EXPECT_EQ(pen.style(), Qt::SolidLine);
  EXPECT_EQ(pen.width(), constants::kDefaultLineWidth);
}

//! Checking propagation of display name from item to QLineSeries.
TEST_F(LineSeriesControllerTest, DisplayName)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetDisplayName("abc");

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);

  // initial name was propagated
  EXPECT_EQ(line_series.name(), QString("abc"));

  // changing the name and validating update
  line_series_item->SetDisplayName("def");
  EXPECT_EQ(line_series.name(), QString("def"));
}

TEST_F(LineSeriesControllerTest, SetOffset)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetXOffset(100.0);

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);

  EXPECT_EQ(controller.GetItem(), line_series_item);

  // initially there is no points on graph
  EXPECT_TRUE(line_series.points().empty());

  // filling the model with line series data: 3 points
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  // linking to the data item
  line_series_item->SetDataItem(data_item);

  // our graph should get points shifted by offset
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 101.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 102.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
  EXPECT_EQ(qt_points.at(2).x(), 103.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);

  // changing offset and checking new points coordinates
  line_series_item->SetXOffset(10.0);
  qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 11.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 12.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
  EXPECT_EQ(qt_points.at(2).x(), 13.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

//! Checking propagation of kDisplayed property.
TEST_F(LineSeriesControllerTest, Displayed)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetDisplayed(false);

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);

  // initial displayed property was propagated
  EXPECT_FALSE(line_series.isVisible());

  // changing visibility and checking its propagation to Qt
  line_series_item->SetDisplayed(true);
  EXPECT_TRUE(line_series.isVisible());
}

TEST_F(LineSeriesControllerTest, UpdateActivePointCoordinate)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();

  // filling the model with line series data: 3 points
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);
  EXPECT_EQ(controller.GetItem(), line_series_item);

  // our graph should get points
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);

  // simulating press on second point
  emit line_series.pressed({2.0, 20.0});

  // simulating moving the second point to new coordinates (5.0, 50.0)
  controller.UpdateActivePointCoordinate(5.0, 50.0);
  qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 5.0);
  EXPECT_EQ(qt_points.at(1).y(), 50.0);
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);

  // simulating the release of second point
  emit line_series.released({5.0, 50.0});

  // simulating moving to another place, coordinates should remain the same
  controller.UpdateActivePointCoordinate(50.0, 500.0);
  qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 5.0);
  EXPECT_EQ(qt_points.at(1).y(), 50.0);
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

TEST_F(LineSeriesControllerTest, SetMarkerSize)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetMarkerSize(7.5);

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  EXPECT_DOUBLE_EQ(line_series.markerSize(), 7.5);
#else
  EXPECT_DOUBLE_EQ(3.0, LineSeriesItem::kDefaultMarkerSize);
#endif

  // changing marker size and checking its propagation to Qt
  line_series_item->SetMarkerSize(12.0);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  EXPECT_DOUBLE_EQ(line_series.markerSize(), 12.0);
#else
  EXPECT_DOUBLE_EQ(3.0, LineSeriesItem::kDefaultMarkerSize);
#endif
}

TEST_F(LineSeriesControllerTest, SetPointsVisible)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetPointsVisible(false);

  QLineSeries line_series;
  LineSeriesController controller(&line_series);

  controller.SetItem(line_series_item);

  // initial points visible property was propagated
  EXPECT_FALSE(line_series.pointsVisible());

  // changing visibility and checking its propagation to Qt
  line_series_item->SetPointsVisible(true);
  EXPECT_TRUE(line_series.pointsVisible());
}

}  // namespace mvvm::test
