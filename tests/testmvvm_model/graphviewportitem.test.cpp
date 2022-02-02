// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/standarditems/graphviewportitem.h"

#include "mockitemlistener.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/axisitems.h"
#include "mvvm/standarditems/data1ditem.h"
#include "mvvm/standarditems/graphitem.h"

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Testing AxesItems.

class GraphViewportItemTest : public ::testing::Test
{
};

//! Initial state.

TEST_F(GraphViewportItemTest, initialState)
{
  GraphViewportItem item;
  EXPECT_EQ(item.xAxis()->GetType(), ViewportAxisItem::Type);
  EXPECT_EQ(item.yAxis()->GetType(), ViewportAxisItem::Type);
  EXPECT_EQ(item.GetGraphItems().size(), 0);
}

//! Add graph to viewport.

TEST_F(GraphViewportItemTest, addItem)
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
  viewport_item->setViewportToContent();

  // x-axis of viewport should be set to FixedBinAxis of DataItem
  auto xaxis = viewport_item->xAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), expected_centers[0]);
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), expected_centers[2]);

  // y-axis of viewport should be set to min/max of expected_content
  auto yaxis = viewport_item->yAxis();
  auto [expected_amin, expected_amax] =
      std::minmax_element(std::begin(expected_values), std::end(expected_values));
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), *expected_amin);
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), *expected_amax);
}

//! Check signaling on set data item.

TEST_F(GraphViewportItemTest, onAddItem)
{
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();

  MockItemListener widget(viewport_item);

  const TagIndex expected_tagrow{ViewportItem::T_ITEMS, 0};
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnItemInserted(viewport_item, expected_tagrow)).Times(1);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);

  // triggering action
  auto graph = model.InsertItem<GraphItem>(viewport_item);
}

//! Check signaling on set data item.

TEST_F(GraphViewportItemTest, onSetDataItem)
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

  MockItemListener widget(viewport_item);

  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);
  //  EXPECT_CALL(widget, onChildPropertyChange(graph_item, GraphItem::P_LINK)).Times(1); // FIXME
  //  after onChildPropertyChange restoration
  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);

  // triggering action
  graph_item->SetDataItem(data_item);
}

//! Add graph to viewport.

TEST_F(GraphViewportItemTest, setViewportToContentWithMargins)
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
  const double bottom{0.1}, top{0.1};
  viewport_item->setViewportToContent(0.0, top, 0.0, bottom);

  // x-axis of viewport should be set to FixedBinAxis of DataItem
  auto xaxis = viewport_item->xAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), expected_centers[0]);
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), expected_centers[2]);

  // y-axis of viewport should be set to min/max of expected_content
  auto yaxis = viewport_item->yAxis();
  auto [expected_amin, expected_amax] =
      std::minmax_element(std::begin(expected_values), std::end(expected_values));

  double expected_ymin = *expected_amin - (*expected_amax - *expected_amin) * bottom;
  double expected_ymax = *expected_amax + (*expected_amax - *expected_amin) * top;
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), expected_ymin);
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), expected_ymax);
}
