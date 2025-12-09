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
#include <mvvm/standarditems/point_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace mvvm::test
{

class LineSeriesDataControllerTest : public ::testing::Test
{
};

TEST_F(LineSeriesDataControllerTest, InitialState)
{
  EXPECT_THROW(LineSeriesDataController{nullptr}, RuntimeException);

  QLineSeries line_series;
  const LineSeriesDataController controller(&line_series);

  EXPECT_EQ(controller.GetQtLineSeries(), &line_series);
  EXPECT_EQ(controller.GetDataItem(), nullptr);
  EXPECT_EQ(controller.GetActivePoint(), nullptr);
}

TEST_F(LineSeriesDataControllerTest, SetEmptyLineSeriesDataItem)
{
  {  // attempt to set item without the model
    const LineSeriesDataItem data_item;
    QLineSeries line_series;
    LineSeriesDataController controller(&line_series);
    EXPECT_THROW(controller.SetItem(&data_item), RuntimeException);
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

TEST_F(LineSeriesDataControllerTest, SetNonEmptyLineSeriesDataItem)
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

TEST_F(LineSeriesDataControllerTest, ValidateLineSeriesOnCoordinateUpdate)
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

TEST_F(LineSeriesDataControllerTest, ValidateLineSeriesOnPointRemoved)
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

TEST_F(LineSeriesDataControllerTest, ValidateLineSeriesOnPointInserted)
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

  const QSignalSpy spy_points_replaced(&line_series, &QLineSeries::pointsReplaced);

  controller.SetXOffset(10.0);

  EXPECT_EQ(spy_points_replaced.count(), 1);

  // validating (x,y) of points in QLineSeries
  auto qt_points = line_series.points();
  EXPECT_EQ(qt_points.at(0).x(), 11.0);
  EXPECT_EQ(qt_points.at(0).y(), 10.0);
  EXPECT_EQ(qt_points.at(1).x(), 12.0);
  EXPECT_EQ(qt_points.at(1).y(), 20.0);
}

TEST_F(LineSeriesDataControllerTest, ActivePointOnPointPressed)
{
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  // simulating mouse press on first point
  emit controller.GetQtLineSeries()->pressed({1.0, 10.0});
  const auto active_point = controller.GetActivePoint();
  ASSERT_NE(active_point, nullptr);
  EXPECT_EQ(active_point->GetX(), 1.0);
  EXPECT_EQ(active_point->GetY(), 10.0);

  // simulating mouse release
  emit controller.GetQtLineSeries()->released({1.0, 10.0});
  EXPECT_EQ(controller.GetActivePoint(), nullptr);
}

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
TEST_F(LineSeriesDataControllerTest, InvestigatingHowQtSelectPoints)
{
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  const QSignalSpy spy_selected_point_changed(&line_series, &QLineSeries::selectedPointsChanged);
  line_series.setPointSelected(0, true);

  EXPECT_EQ(spy_selected_point_changed.count(), 1);
  EXPECT_TRUE(line_series.isPointSelected(0));

  // selecting the same again
  line_series.setPointSelected(0, true);
  EXPECT_EQ(spy_selected_point_changed.count(), 1);  // no new signal
}

TEST_F(LineSeriesDataControllerTest, SetPointsSelected)
{
  mvvm::ApplicationModel model;

  auto data_item = std::make_unique<LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});
  auto data_item_ptr = data_item.get();
  model.InsertItem(std::move(data_item), model.GetRootItem(), mvvm::TagIndex::Append());

  // setting up controller
  QLineSeries line_series;
  LineSeriesDataController controller(&line_series);
  controller.SetItem(data_item_ptr);

  const QSignalSpy spy_selected_point_changed(&line_series, &QLineSeries::selectedPointsChanged);

  const std::vector<PointItem*> selection1({data_item_ptr->GetPoint(0)});
  controller.SetPointsSelected(selection1);

  EXPECT_EQ(spy_selected_point_changed.count(), 1);

  // EXPECT_TRUE(data_item_ptr->IsPointSelected(0));
  EXPECT_TRUE(line_series.isPointSelected(0));

  // EXPECT_FALSE(data_item_ptr->IsPointSelected(1));
  EXPECT_FALSE(line_series.isPointSelected(1));

  // selecting same point again
  controller.SetPointsSelected(selection1);
  EXPECT_EQ(spy_selected_point_changed.count(), 1);  // no new signal

  // adding second point to selection
  const std::vector<PointItem*> selection2({data_item_ptr->GetPoint(0), data_item_ptr->GetPoint(1)});
  controller.SetPointsSelected(selection2);
  EXPECT_EQ(spy_selected_point_changed.count(), 2);
  EXPECT_TRUE(line_series.isPointSelected(0));
  EXPECT_TRUE(line_series.isPointSelected(1));
}

#endif

}  // namespace mvvm::test
