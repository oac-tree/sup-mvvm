/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/plotting/customplot/mouse_move_reporter.h"

#include <mvvm/model/application_model.h>
#include <mvvm/plotting/customplot/graph_viewport_plot_controller.h>
#include <mvvm/plotting/customplot/mouse_pos_info.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QMouseEvent>

namespace mvvm::test
{

/**
 * @brief Tests for MouseMoveReporter class.
 */
class MouseMoveReporterTest : public ::testing::Test
{
public:
};

TEST_F(MouseMoveReporterTest, Construct)
{
  EXPECT_THROW(MouseMoveReporter(nullptr, {}), RuntimeException);

  auto custom_plot = std::make_unique<QCustomPlot>();
  EXPECT_THROW(MouseMoveReporter(custom_plot.get(), {}), RuntimeException);
}

TEST_F(MouseMoveReporterTest, MouseMoveReporter)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  GraphViewportPlotController viewport_controller(custom_plot.get());

  // setting up controller with viewport item
  ApplicationModel model;
  auto viewport_item = model.InsertItem<GraphViewportItem>();

  auto data_item = model.InsertItem<Data1DItem>();
  const std::vector<double> expected_values = {1.0, 2.0, 3.0};
  const std::vector<double> expected_centers = {0.5, 1.5, 2.5};
  data_item->SetAxis<FixedBinAxisItem>(3U, 0.0, 3.0);
  data_item->SetValues(expected_values);

  auto graph_item = model.InsertItem<GraphItem>(viewport_item);
  graph_item->SetDataItem(data_item);
  viewport_controller.SetItem(viewport_item);

  EXPECT_EQ(custom_plot->graphCount(), 1);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().lower, expected_centers[0]);
  EXPECT_DOUBLE_EQ(custom_plot->xAxis->range().upper, expected_centers[2]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().lower, expected_values[0]);
  EXPECT_DOUBLE_EQ(custom_plot->yAxis->range().upper, expected_values[2]);

  ::testing::MockFunction<void(const MousePosInfo&)> mock_callback;
  MouseMoveReporter reporter(custom_plot.get(), mock_callback.AsStdFunction());
  EXPECT_CALL(mock_callback, Call(testing::_)).Times(1);
  QMouseEvent event(QEvent::MouseMove, QPointF(1, 1), QPointF(1, 1), Qt::NoButton, {},
                    Qt::NoModifier);
  QApplication::sendEvent(custom_plot.get(), &event);

  QCoreApplication::processEvents();
}

}  // namespace mvvm::test
