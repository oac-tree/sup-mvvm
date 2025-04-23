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

#include "mvvm/plotting/charts/line_series_data_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

class LineSeriesDataControllerTest : public ::testing::Test
{
};

TEST_F(LineSeriesDataControllerTest, InitialState)
{
  EXPECT_THROW(LineSeriesDataController{nullptr}, std::runtime_error);

  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);

  EXPECT_EQ(controller.GetQtLineSeries(), &line_series);
  EXPECT_EQ(controller.GetDataItem(), nullptr);
}

//! Validating SetItem method.

TEST_F(LineSeriesDataControllerTest, SetEmptyItem)
{
  {  // attempt to set item without the model
    LineSeriesDataItem data_item;
    QLineSeries line_series;
    LineSeriesDataController controller(&line_series);
    EXPECT_THROW(controller.SetItem(&data_item), std::runtime_error);
  }

  {  // setting data item without points
    mvvm::ApplicationModel model;
    auto item = model.InsertItem<LineSeriesDataItem>(model.GetRootItem(), mvvm::TagIndex::Append());

    QLineSeries line_series;
    LineSeriesDataController controller(&line_series);
    controller.SetItem(item);
    EXPECT_EQ(controller.GetDataItem(), item);

    controller.SetItem(nullptr);
    EXPECT_EQ(controller.GetDataItem(), nullptr);
    EXPECT_EQ(line_series.count(), 0);
  }
}

//! Validating SetItem method and initial initialization of time series from non-empty
//! AnyValueArrayItem.
TEST_F(LineSeriesDataControllerTest, SetItem)
{
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto data_item_ptr = data_item.get();

  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);
  EXPECT_EQ(controller.GetDataItem(), data_item_ptr);

  EXPECT_EQ(line_series.count(), 3);

  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

//! Validation QLineSeries update on data change event.

TEST_F(LineSeriesDataControllerTest, OnDataChanged)
{
  // filling the model with line series data: 3 points
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  QSignalSpy spy_point_replaced(&line_series, &QLineSeries::pointReplaced);

  // changing the value of second point
  data_item_ptr->SetPointCoordinates(1, {2.0, 25.0});

  // expecting notification on Qt side, index of replaced point
  EXPECT_EQ(mvvm::test::GetSendItem<int>(spy_point_replaced), 1);

  // validating (x,y) of points in QLineSeries
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 25.0);  // changed value
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

TEST_F(LineSeriesDataControllerTest, OnPointRemoved)
{
  // filling the model with line series data: 3 points
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  QSignalSpy spy_point_removed(&line_series, &QLineSeries::pointRemoved);

  // removing middle point
  data_item_ptr->RemovePoint(1);

  // expecting notification on Qt side, index of removed point
  EXPECT_EQ(mvvm::test::GetSendItem<int>(spy_point_removed), 1);

  // validating (x,y) of points in QLineSeries
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 3.0);
  EXPECT_EQ(qt_points.at(1).y(), 30.0);
}

TEST_F(LineSeriesDataControllerTest, OnPointInserted)
{
  mvvm::ApplicationModel model;

  // filling the model with line series data: 2 points
  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {3.0, 30.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  QSignalSpy spy_point_inserted(&line_series, &QLineSeries::pointAdded);

  // inserting point in the middle
  data_item_ptr->InsertPoint(1, {2.0, 20.0});

  // expecting notification on Qt side, index of inserted point
  EXPECT_EQ(mvvm::test::GetSendItem<int>(spy_point_inserted), 1);

  // validating (x,y) of points in QLineSeries
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 1.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 2.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);  // added value
  EXPECT_EQ(qt_points.at(2).x(), 3.0);
  EXPECT_EQ(qt_points.at(2).y(), 30.0);
}

TEST_F(LineSeriesDataControllerTest, SetXOffset)
{
  // filling the model with line series data: 3 points
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  QSignalSpy spy_points_replaced(&line_series, &QLineSeries::pointsReplaced);

  controller.SetXOffset(10.0);

  EXPECT_EQ(spy_points_replaced.count(), 1);

  // validating (x,y) of points in QLineSeries
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 11.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 12.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
}
