/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/graphitem.h"

#include "mockitemlistener.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/axisitems.h"
#include "mvvm/standarditems/data1ditem.h"
#include "mvvm/standarditems/plottableitems.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;
using ::testing::_;

//! Testing GraphItem.

class GraphItemTest : public ::testing::Test
{
};

//! Initial state.

TEST_F(GraphItemTest, InitialState)
{
  GraphItem item;
  EXPECT_TRUE(item.GetDataItem() == nullptr);
  EXPECT_EQ(item.GetBinCenters(), std::vector<double>{});
  EXPECT_EQ(item.GetValues(), std::vector<double>{});
  EXPECT_EQ(item.GetErrors(), std::vector<double>{});
  EXPECT_EQ(item.GetNamedColor(), std::string("black"));
}

//! Setting dataItem in model context.

TEST_F(GraphItemTest, SetDataItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  graph_item->SetDataItem(data_item);

  EXPECT_EQ(graph_item->GetDataItem(), data_item);
}

//! Setting dataItem in model context.

TEST_F(GraphItemTest, GetValues)
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

TEST_F(GraphItemTest, GetErrors)
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

TEST_F(GraphItemTest, SetNullData)
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

TEST_F(GraphItemTest, OnSetDataItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  MockItemListener widget(graph_item);

  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(graph_item, GraphItem::P_LINK)).Times(1);
  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);

  // performing action
  graph_item->SetDataItem(data_item);
}

//! Sets GraphItem from another GraphItem

TEST_F(GraphItemTest, SetFromGraphItem)
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

TEST_F(GraphItemTest, SetNamedColor)
{
  GraphItem item;
  item.SetNamedColor("mediumaquamarine");
  EXPECT_EQ(item.GetNamedColor(), std::string("mediumaquamarine"));
}
