/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/charts/axis_plot_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/signals/event_types.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/test/mock_item_listener.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QtCharts/QValueAxis>

using namespace mvvm;

//! Testing AxisPlotController class.

class AxisPlotControllerTests : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;
};

//! Initial state of the controller and axes after setup.

TEST_F(AxisPlotControllerTests, InitialState)
{
  QtCharts::QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  AxisPlotController controller(&axis);
  EXPECT_EQ(controller.GetQtAxis(), &axis);
  EXPECT_EQ(controller.GetItem(), nullptr);

  controller.SetItem(axis_item);
  EXPECT_EQ(controller.GetItem(), axis_item);

  EXPECT_EQ(axis_item->GetMin(), 0.0);
  EXPECT_EQ(axis_item->GetMax(), 1.0);

  EXPECT_EQ(axis.min(), 0.0);
  EXPECT_EQ(axis.max(), 1.0);
}

//! Testing update of QtCharts::QValueAxis when item range was changed.

TEST_F(AxisPlotControllerTests, SetItemRange)
{
  QtCharts::QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  AxisPlotController controller(&axis);
  controller.SetItem(axis_item);

  QSignalSpy spy_range_changed(&axis, &QtCharts::QValueAxis::rangeChanged);

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

//! Testing update of ViewportAxisItem when QtCharts::QValueAxis axis has changed.

TEST_F(AxisPlotControllerTests, SetQtQxisRange)
{
  QtCharts::QValueAxis axis;

  mvvm::ApplicationModel model;
  auto axis_item = model.InsertItem<mvvm::ViewportAxisItem>();

  AxisPlotController controller(&axis);
  controller.SetItem(axis_item);

  QSignalSpy spy_range_changed(&axis, &QtCharts::QValueAxis::rangeChanged);

  mock_listener_t widget(axis_item);
  mvvm::PropertyChangedEvent expected_event{axis_item, mvvm::ViewportAxisItem::kMax};
  EXPECT_CALL(widget, OnPropertyChanged(expected_event)).Times(1);

  axis.setMax(5.0);

  EXPECT_EQ(spy_range_changed.count(), 1);
  EXPECT_EQ(axis_item->GetMin(), 0.0);
  EXPECT_EQ(axis_item->GetMax(), 5.0);
}
