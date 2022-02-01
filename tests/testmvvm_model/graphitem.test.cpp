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

TEST_F(GraphItemTest, initialState)
{
  GraphItem item;
  EXPECT_TRUE(item.dataItem() == nullptr);
  EXPECT_EQ(item.binCenters(), std::vector<double>{});
  EXPECT_EQ(item.binValues(), std::vector<double>{});
  EXPECT_EQ(item.binErrors(), std::vector<double>{});
  EXPECT_EQ(item.colorName(), std::string("black"));
}

//! Setting dataItem in model context.

TEST_F(GraphItemTest, setDataItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  graph_item->setDataItem(data_item);

  EXPECT_EQ(graph_item->dataItem(), data_item);
}

//! Setting dataItem in model context.

TEST_F(GraphItemTest, binValues)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->setDataItem(data_item);

  EXPECT_EQ(graph_item->binValues(), expected_values);
  EXPECT_EQ(graph_item->binCenters(), expected_centers);
}

//! Setting dataItem with errors

TEST_F(GraphItemTest, binErrors)
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

  graph_item->setDataItem(data_item);

  EXPECT_EQ(graph_item->binValues(), expected_values);
  EXPECT_EQ(graph_item->binCenters(), expected_centers);
  EXPECT_EQ(graph_item->binErrors(), expected_errors);
}

//! Check unlinking when nullptr is set as Data1DItem.

TEST_F(GraphItemTest, setNullData)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();

  // preparing data item
  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->setDataItem(data_item);
  EXPECT_EQ(graph_item->dataItem(), data_item);

  // setting null as data item
  graph_item->setDataItem(nullptr);
  EXPECT_TRUE(graph_item->dataItem() == nullptr);
  EXPECT_EQ(graph_item->binCenters(), std::vector<double>{});
  EXPECT_EQ(graph_item->binValues(), std::vector<double>{});
  EXPECT_EQ(graph_item->binErrors(), std::vector<double>{});
}

//! Check signaling on set data item.

TEST_F(GraphItemTest, onSetDataItem)
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
  graph_item->setDataItem(data_item);
}

//! Sets GraphItem from another GraphItem

TEST_F(GraphItemTest, setFromGraphItem)
{
  ApplicationModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto graph_item = model.InsertItem<GraphItem>();
  auto graph_item2 = model.InsertItem<GraphItem>();

  std::vector<double> expected_values = {1.0, 2.0, 3.0};
  std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(expected_values);

  graph_item->setDataItem(data_item);
  graph_item->penItem()->SetNamedColor("red");

  graph_item2->setFromGraphItem(graph_item);

  EXPECT_EQ(graph_item2->binValues(), expected_values);
  EXPECT_EQ(graph_item2->binCenters(), expected_centers);
  EXPECT_EQ(graph_item2->penItem()->GetNamedColor(), "red");
}

TEST_F(GraphItemTest, penItem_setNamedColor)
{
  GraphItem item;
  item.setNamedColor("mediumaquamarine");
  EXPECT_EQ(item.colorName(), std::string("mediumaquamarine"));
}
