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

#include "mvvm/standarditems/graph_viewport_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;
using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;

class GraphViewportItemTests : public ::testing::Test
{
};

TEST_F(GraphViewportItemTests, InitialState)
{
  const GraphViewportItem item;
  EXPECT_EQ(item.GetXAxis()->GetType(), ViewportAxisItem::GetStaticType());
  EXPECT_EQ(item.GetYAxis()->GetType(), ViewportAxisItem::GetStaticType());
  EXPECT_EQ(item.GetGraphItems().size(), 0);
  EXPECT_EQ(item.GetGraphCount(), 0);
}

TEST_F(GraphViewportItemTests, AddGraphToViewport)
{
  ApplicationModel model;

  auto viewport_item = model.InsertItem<GraphViewportItem>();
  auto graph_item = model.InsertItem<GraphItem>(viewport_item);
  auto data_item = model.InsertItem<Data1DItem>();

  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);
  EXPECT_EQ(viewport_item->GetGraphItems().size(), 1);
  EXPECT_EQ(viewport_item->GetGraphCount(), 1);

  // updating viewport to graph
  viewport_item->SetViewportToContent();

  // x-axis of viewport should be set to FixedBinAxis of DataItem
  auto xaxis = viewport_item->GetXAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), expected_centers[0]);
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), expected_centers[2]);

  // y-axis of viewport should be set to min/max of expected_content
  auto yaxis = viewport_item->GetYAxis();
  auto [expected_amin, expected_amax] =
      std::minmax_element(std::begin(expected_values), std::end(expected_values));
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), *expected_amin);
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), *expected_amax);
}

TEST_F(GraphViewportItemTests, OnAddGraphToViewport)
{
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();

  mock_listener_t widget(viewport_item);

  const TagIndex expected_tagrow{ViewportItem::kItems, 0};
  ItemInsertedEvent expected_event{viewport_item, expected_tagrow};
  EXPECT_CALL(widget, OnItemInserted(expected_event)).Times(1);

  // triggering action
  auto graph = model.InsertItem<GraphItem>(viewport_item);
}

TEST_F(GraphViewportItemTests, OnSetDataItem)
{
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();

  // setting up data item
  auto data_item = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  // inserting graph item
  auto graph_item = model.InsertItem<GraphItem>(viewport_item);

  mock_listener_t widget(viewport_item);

  // expect no calls, strickt mock will take care if it's not the case

  // triggering action
  graph_item->SetDataItem(data_item);
}

TEST_F(GraphViewportItemTests, SetViewportToContentWithMargins)
{
  SessionModel model;

  auto viewport_item = model.InsertItem<GraphViewportItem>();
  auto graph_item = model.InsertItem<GraphItem>(viewport_item);
  auto data_item = model.InsertItem<Data1DItem>();

  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);
  EXPECT_EQ(viewport_item->GetGraphItems().size(), 1);

  // updating viewport to graph
  const double bottom{0.1};
  const double top{0.1};
  viewport_item->SetViewportToContent(0.0, top, 0.0, bottom);

  // x-axis of viewport should be set to FixedBinAxis of DataItem
  auto xaxis = viewport_item->GetXAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), expected_centers[0]);
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), expected_centers[2]);

  // y-axis of viewport should be set to min/max of expected_content
  auto yaxis = viewport_item->GetYAxis();
  auto [expected_amin, expected_amax] =
      std::minmax_element(std::begin(expected_values), std::end(expected_values));

  const double expected_ymin = *expected_amin - (*expected_amax - *expected_amin) * bottom;
  const double expected_ymax = *expected_amax + (*expected_amax - *expected_amin) * top;
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), expected_ymin);
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), expected_ymax);
}
