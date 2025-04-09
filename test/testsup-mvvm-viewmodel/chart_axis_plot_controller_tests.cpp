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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/charts/chart_axis_plot_controller.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/signals/event_types.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/plottable_items.h>
#include <mvvm/test/mock_item_listener.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

/**
 * @brief Tests for ChartAxisPlotController class.
 */
class ChartAxisPlotControllerTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;
};

//! Initial state of the controller and axes after setup.
TEST_F(ChartAxisPlotControllerTest, InitialState)
{
  QValueAxis axis;

  EXPECT_THROW(ChartAxisPlotController(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  ChartAxisPlotController controller(&axis);
  EXPECT_EQ(controller.GetQtAxis(), &axis);
  EXPECT_EQ(controller.GetItem(), nullptr);

  controller.SetItem(axis_item);
  EXPECT_EQ(controller.GetItem(), axis_item);

  EXPECT_EQ(axis_item->GetMin(), 0.0);
  EXPECT_EQ(axis_item->GetMax(), 1.0);

  EXPECT_EQ(axis.min(), 0.0);
  EXPECT_EQ(axis.max(), 1.0);
}

//! Testing update of QValueAxis when item range was changed.
TEST_F(ChartAxisPlotControllerTest, SetItemRange)
{
  QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  ChartAxisPlotController controller(&axis);
  controller.SetItem(axis_item);

  QSignalSpy spy_range_changed(&axis, &QValueAxis::rangeChanged);

  // We expect only  one event on max change. This confirms that controller doesn't create any
  // update loops.
  mock_listener_t widget(axis_item);
  mvvm::PropertyChangedEvent expected_event{axis_item, mvvm::ViewportAxisItem::kMax};
  EXPECT_CALL(widget, OnPropertyChanged(expected_event)).Times(1);

  axis_item->SetMax(5.0);

  EXPECT_EQ(spy_range_changed.count(), 1);
  EXPECT_EQ(axis.min(), 0.0);
  EXPECT_EQ(axis.max(), 5.0);
}

//! Testing update of ViewportAxisItem when QValueAxis axis has changed.
TEST_F(ChartAxisPlotControllerTest, SetQtQxisRange)
{
  QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  ChartAxisPlotController controller(&axis);
  controller.SetItem(axis_item);

  QSignalSpy spy_range_changed(&axis, &QValueAxis::rangeChanged);

  mock_listener_t widget(axis_item);
  mvvm::PropertyChangedEvent expected_event{axis_item, mvvm::ViewportAxisItem::kMax};
  EXPECT_CALL(widget, OnPropertyChanged(expected_event)).Times(1);

  axis.setMax(5.0);

  EXPECT_EQ(spy_range_changed.count(), 1);
  EXPECT_EQ(axis_item->GetMin(), 0.0);
  EXPECT_EQ(axis_item->GetMax(), 5.0);
}

//! Changing axis title via item.
TEST_F(ChartAxisPlotControllerTest, ChangeAxisTitle)
{
  QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  ChartAxisPlotController controller(&axis);
  controller.SetItem(axis_item);

  EXPECT_EQ(axis.titleText(), QString(""));

  axis_item->GetTitleItem()->SetText("abc");
  EXPECT_EQ(axis.titleText(), QString("abc"));
}
