/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/graph_item.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_item_listener.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/plottable_items.h>

#include <QColor>

using namespace mvvm;
using ::testing::_;

//! Testing GraphItem.

class GraphItemTests : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<testutils::MockItemListener>;
};

//! Initial state.

TEST_F(GraphItemTests, InitialState)
{
  GraphItem item;
  EXPECT_TRUE(item.GetDataItem() == nullptr);
  EXPECT_EQ(item.GetBinCenters(), std::vector<double>{});
  EXPECT_EQ(item.GetValues(), std::vector<double>{});
  EXPECT_EQ(item.GetErrors(), std::vector<double>{});
  EXPECT_EQ(item.GetNamedColor(), std::string("black"));
}

//! Setting dataItem in model context.

TEST_F(GraphItemTests, SetDataItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  graph_item->SetDataItem(data_item);

  EXPECT_EQ(graph_item->GetDataItem(), data_item);
}

//! Setting dataItem in model context.

TEST_F(GraphItemTests, GetValues)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);

  EXPECT_EQ(graph_item->GetValues(), expected_values);
  EXPECT_EQ(graph_item->GetBinCenters(), expected_centers);
}

//! Setting dataItem with errors

TEST_F(GraphItemTests, GetErrors)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  std::vector<double> expected_errors = {0.1, 0.2, 0.3};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);
  data_item->SetErrors(expected_errors);

  graph_item->SetDataItem(data_item);

  EXPECT_EQ(graph_item->GetValues(), expected_values);
  EXPECT_EQ(graph_item->GetBinCenters(), expected_centers);
  EXPECT_EQ(graph_item->GetErrors(), expected_errors);
}

//! Check unlinking when nullptr is set as Data1DItem.

TEST_F(GraphItemTests, SetNullData)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  // preparing data item
  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);
  EXPECT_EQ(graph_item->GetDataItem(), data_item);

  // setting null as data item
  graph_item->SetDataItem(nullptr);
  EXPECT_TRUE(graph_item->GetDataItem() == nullptr);
  EXPECT_EQ(graph_item->GetBinCenters(), std::vector<double>{});
  EXPECT_EQ(graph_item->GetValues(), std::vector<double>{});
  EXPECT_EQ(graph_item->GetErrors(), std::vector<double>{});
}

//! Check signaling on set data item.

TEST_F(GraphItemTests, OnSetDataItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  mock_listener_t widget(graph_item);

  PropertyChangedEvent expected_event{graph_item, "kLink"};
  EXPECT_CALL(widget, OnEvent(event_variant_t(expected_event))).Times(1);

  // performing action
  graph_item->SetDataItem(data_item);
}

//! Sets GraphItem from another GraphItem

TEST_F(GraphItemTests, SetFromGraphItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();
  auto graph_item2 = model.InsertItem<GraphItem>();

  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->SetDataItem(data_item);
  graph_item->GetPenItem()->SetNamedColor("red");

  graph_item2->SetFromGraphItem(graph_item);

  EXPECT_EQ(graph_item2->GetValues(), expected_values);
  EXPECT_EQ(graph_item2->GetBinCenters(), expected_centers);
  EXPECT_EQ(graph_item2->GetPenItem()->GetNamedColor(), "red");
}

TEST_F(GraphItemTests, SetNamedColor)
{
  GraphItem item;
  item.SetNamedColor("mediumaquamarine");
  EXPECT_EQ(item.GetNamedColor(), std::string("mediumaquamarine"));
}
