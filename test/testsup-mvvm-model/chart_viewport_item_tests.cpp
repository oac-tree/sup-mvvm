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

#include "mvvm/standarditems/chart_viewport_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing ChartViewportItem class.

class ChartViewportItemTest : public ::testing::Test
{
};

TEST_F(ChartViewportItemTest, ViewportAxisInitialState)
{
  mvvm::ApplicationModel model;

  auto viewport_item = model.InsertItem<ChartViewportItem>();

  EXPECT_EQ(viewport_item->GetLineSeriesCount(), 0);
  EXPECT_NE(viewport_item->GetXAxis(), nullptr);
  EXPECT_NE(viewport_item->GetYAxis(), nullptr);
}

TEST_F(ChartViewportItemTest, SingleLineSeries)
{
  mvvm::ApplicationModel model;

  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  EXPECT_NE(viewport_item->GetXAxis(), nullptr);
  EXPECT_NE(viewport_item->GetYAxis(), nullptr);

  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 1.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 1.0);

  EXPECT_EQ(viewport_item->GetLineSeries(), std::vector<LineSeriesItem*>({line_series_item}));
  EXPECT_EQ(viewport_item->GetLineSeriesCount(), 1);
}

TEST_F(ChartViewportItemTest, SetViewportToContent)
{
  mvvm::ApplicationModel model;

  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  line_series_item->SetDataItem(data_item);

  // setting viewport to content when viewport is empty
  viewport_item->SetViewportToContent();
  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 1.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 1.0);

  // adding points to the model, make LineSeriesLooking at it
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 25.0}, {3.0, 10.0}});

  // setting viewport to content
  viewport_item->SetViewportToContent();

  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 1.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 3.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 10.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 25.0);
}

TEST_F(ChartViewportItemTest, SetViewportToContentTwoLineSeriesWithOffset)
{
  mvvm::ApplicationModel model;

  // creating viewport
  auto viewport_item = model.InsertItem<ChartViewportItem>();

  auto line_series_item0 = model.InsertItem<LineSeriesItem>(viewport_item);
  auto data_item0 = model.InsertItem<LineSeriesDataItem>();
  line_series_item0->SetDataItem(data_item0);
  data_item0->SetWaveform({{1.0, 10.0}, {2.0, 20.0}});
  line_series_item0->SetXOffset(10.0);

  auto line_series_item1 = model.InsertItem<LineSeriesItem>(viewport_item);
  auto data_item1 = model.InsertItem<LineSeriesDataItem>();
  line_series_item1->SetDataItem(data_item1);
  data_item1->SetWaveform({{1.0, 12.0}, {3.0, 30.0}});
  line_series_item1->SetXOffset(20.0);

  // setting viewport to content
  viewport_item->SetViewportToContent();

  // x-axis of viewport should be set to xmin, xmax between two graphs, with offset taken into
  // accoun
  auto xaxis = viewport_item->GetXAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), 11.0);  // xmin of first item (+offset 10)
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), 23.0);  // xmax of second item  (offset 20)

  // y-axis of viewport should be set to xmin, xmax between two graphs, with offset taken into
  // accoun
  auto yaxis = viewport_item->GetYAxis();
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), 10.0);  // ymin of first item
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), 30.0);  // ymax of second item
}
