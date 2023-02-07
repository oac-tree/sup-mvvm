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

#include "mvvm/plotting/viewport_axis_plot_controller.h"

#include <gtest/gtest.h>
#include <qcustomplot.h>
#include <testutils/mock_item_listener.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/plottable_items.h>

#include <QSignalSpy>

using namespace mvvm;
using ::testing::_;

//! Testing AxisPlotControllers.

Q_DECLARE_METATYPE(QCPRange)

class ViewportAxisPlotControllerTests : public ::testing::Test
{
public:
  std::unique_ptr<QSignalSpy> CreateSpy(QCPAxis* axis)
  {
    return std::make_unique<QSignalSpy>(
        axis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged));
  }

  std::unique_ptr<QSignalSpy> CreateSpyDoubleRange(QCPAxis* axis)
  {
    return std::make_unique<QSignalSpy>(
        axis,
        static_cast<void (QCPAxis::*)(const QCPRange&, const QCPRange&)>(&QCPAxis::rangeChanged));
  }

  using mock_listener_t = ::testing::StrictMock<testutils::MockItemListener>;
};

//! Initial state.

TEST_F(ViewportAxisPlotControllerTests, InitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  auto axis = custom_plot->xAxis;

  // checking initial defaults
  const double customplot_default_lower(0.0);
  const double customplot_default_upper(5.0);
  EXPECT_EQ(axis->range().lower, customplot_default_lower);
  EXPECT_EQ(axis->range().upper, customplot_default_upper);

  // controller shouldn''t change axis range
  ViewportAxisPlotController controller(axis);
  EXPECT_EQ(axis->range().lower, customplot_default_lower);
  EXPECT_EQ(axis->range().upper, customplot_default_upper);

  // checking axis signaling
  auto xChanged = CreateSpy(custom_plot->xAxis);
  auto yChanged = CreateSpy(custom_plot->yAxis);

  // changing range of axis
  custom_plot->xAxis->setRangeLower(1.0);

  // checking that QCPaxis properly emiting signals
  EXPECT_EQ(xChanged->count(), 1);
  EXPECT_EQ(yChanged->count(), 0);
}

//! Controller subscribed to ViewportAxisItem.
//! Checking that QCPAxis get same parameters as in AxisItem.

TEST_F(ViewportAxisPlotControllerTests, CetViewportAxisItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  const double expected_min = 1.0;
  const double expected_max = 2.0;

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(expected_min);
  axis_item->SetMax(expected_max);

  // setting up QCustomPlot and item controller.
  ASSERT_TRUE(custom_plot->xAxis != nullptr);
  ViewportAxisPlotController controller(custom_plot->xAxis);

  auto prev_y_range = custom_plot->yAxis->range();
  auto xChanged = CreateSpy(custom_plot->xAxis);
  auto yChanged = CreateSpy(custom_plot->yAxis);

  // Subscribtion to ViewportAxisItem should change QCPAxis range for X.
  controller.SetItem(axis_item);

  EXPECT_EQ(custom_plot->xAxis->range().lower, expected_min);
  EXPECT_EQ(custom_plot->xAxis->range().upper, expected_max);
  EXPECT_EQ(xChanged->count(), 1);
  EXPECT_EQ(yChanged->count(), 0);

  // Range for QCPAxis y-axis should stay the same.
  EXPECT_EQ(custom_plot->yAxis->range(), prev_y_range);
}

//! Controller subscribed to ViewportAxisItem.
//! Change QCPAxis and check that ViewportAxisItem got new values.

TEST_F(ViewportAxisPlotControllerTests, ChangeQCPAxis)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(42.0);
  axis_item->SetMax(42.1);

  // setting up QCustomPlot and item controller.
  const double expected_min = 1.0;
  const double expected_max = 2.0;
  auto xChanged = CreateSpy(custom_plot->xAxis);
  auto yChanged = CreateSpy(custom_plot->yAxis);

  // Setting up controller.
  ViewportAxisPlotController controller(custom_plot->xAxis);
  controller.SetItem(axis_item);

  EXPECT_EQ(xChanged->count(), 1);
  EXPECT_EQ(yChanged->count(), 0);

  // Changing QCPAxis
  custom_plot->xAxis->setRange(expected_min, expected_max);
  EXPECT_EQ(xChanged->count(), 2);
  EXPECT_EQ(yChanged->count(), 0);

  // Check changed properties in ViewportAxisItem
  EXPECT_EQ(axis_item->GetMin(), expected_min);
  EXPECT_EQ(axis_item->GetMax(), expected_max);
}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem and check that QCPAxis got new values.

TEST_F(ViewportAxisPlotControllerTests, ChangeViewportAxisItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(42.0);
  axis_item->SetMax(42.1);

  // setting up QCustomPlot and item controller.
  ViewportAxisPlotController controller(custom_plot->xAxis);
  controller.SetItem(axis_item);
  auto xChanged = CreateSpy(custom_plot->xAxis);
  auto yChanged = CreateSpy(custom_plot->yAxis);

  // changing values
  const double expected_min = 1.0;
  const double expected_max = 2.0;
  axis_item->SetMin(expected_min);
  axis_item->SetMax(expected_max);

  // Checking QCPAxis
  EXPECT_EQ(xChanged->count(), 2);
  EXPECT_EQ(yChanged->count(), 0);
  EXPECT_EQ(custom_plot->xAxis->range().lower, expected_min);
  EXPECT_EQ(custom_plot->xAxis->range().upper, expected_max);
}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem and check that QCPAxis got new values.
//! Check correctness of signals issued by QCPAxisItem.

TEST_F(ViewportAxisPlotControllerTests, ChangeViewportAxisItemSignaling)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(1.0);
  axis_item->SetMax(2.0);

  // setting up QCustomPlot and item controller.
  ViewportAxisPlotController controller(custom_plot->xAxis);
  controller.SetItem(axis_item);

  // initial condition
  EXPECT_EQ(custom_plot->xAxis->range().lower, 1.0);
  EXPECT_EQ(custom_plot->xAxis->range().upper, 2.0);

  auto rangeChanged = CreateSpy(custom_plot->xAxis);
  auto rangeChanged2 = CreateSpyDoubleRange(custom_plot->xAxis);

  // making a change
  const double expected_max = 20.0;
  axis_item->SetMax(expected_max);

  // Checking QCPAxis
  EXPECT_EQ(rangeChanged->count(), 1);
  EXPECT_EQ(rangeChanged2->count(), 1);
  EXPECT_EQ(custom_plot->xAxis->range().lower, 1.0);
  EXPECT_EQ(custom_plot->xAxis->range().upper, expected_max);

  QList<QVariant> arguments = rangeChanged->takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto reportedRange = arguments.at(0).value<QCPRange>();
  EXPECT_EQ(reportedRange.lower, 1.0);
  EXPECT_EQ(reportedRange.upper, 20.0);

  arguments = rangeChanged2->takeFirst();
  EXPECT_EQ(arguments.size(), 2);
  auto newRange = arguments.at(0).value<QCPRange>();
  auto oldRange = arguments.at(1).value<QCPRange>();
  EXPECT_EQ(newRange.lower, 1.0);
  EXPECT_EQ(newRange.upper, 20.0);
  EXPECT_EQ(oldRange.lower, 1.0);
  EXPECT_EQ(oldRange.upper, 2.0);
}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem and check that QCPAxis got new values.
//! Check correctness that there is not extra looping and item doesn't start changing many times

TEST_F(ViewportAxisPlotControllerTests, ChangeViewportAxisItemMapping)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(1.0);
  axis_item->SetMax(2.0);

  // setting up QCustomPlot and item controller.
  ViewportAxisPlotController controller(custom_plot->xAxis);
  controller.SetItem(axis_item);

  mock_listener_t widget(axis_item);
  EXPECT_CALL(widget, OnPropertyChanged(axis_item, ViewportAxisItem::kMax)).Times(1);

  // making a change
  const double expected_max = 20.0;
  axis_item->SetMax(expected_max);

  EXPECT_EQ(custom_plot->xAxis->range().lower, 1.0);
  EXPECT_EQ(custom_plot->xAxis->range().upper, expected_max);
}

//! Set ViewportAxisItem logz, subscribe controller and check that QCPAxis has it.

TEST_F(ViewportAxisPlotControllerTests, ViewportLogz)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetInLog(true);

  // setting up QCustomPlot and item controller.
  auto qcp_axis = custom_plot->xAxis;
  ViewportAxisPlotController controller(qcp_axis);
  controller.SetItem(axis_item);

  // QCPAxis should switch to logarithmic
  EXPECT_EQ(qcp_axis->scaleType(), QCPAxis::stLogarithmic);
}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem logz and check that QCPAxis got new values.

TEST_F(ViewportAxisPlotControllerTests, ChangeViewportLogz)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();

  // setting up QCustomPlot and item controller.
  auto qcp_axis = custom_plot->xAxis;
  ViewportAxisPlotController controller(qcp_axis);
  controller.SetItem(axis_item);

  // initial linear scale of axis
  EXPECT_EQ(qcp_axis->scaleType(), QCPAxis::stLinear);

  // changing scale
  axis_item->SetInLog(true);

  // QCPAxis should switch to logarithmic
  EXPECT_EQ(qcp_axis->scaleType(), QCPAxis::stLogarithmic);
}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem and check that QCPAxis got new values.
//! Same test as before, only QCPAxis y-axis checked.

TEST_F(ViewportAxisPlotControllerTests, ChangeViewportAxisItemYCase)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();
  axis_item->SetMin(42.0);
  axis_item->SetMax(42.1);

  // setting up QCustomPlot and item controller.
  ViewportAxisPlotController controller(custom_plot->yAxis);
  controller.SetItem(axis_item);
  auto xChanged = CreateSpy(custom_plot->xAxis);
  auto yChanged = CreateSpy(custom_plot->yAxis);

  // changing values
  const double expected_min = 1.0;
  const double expected_max = 2.0;
  axis_item->SetMin(expected_min);
  axis_item->SetMax(expected_max);

  // Checking QCPAxis
  EXPECT_EQ(xChanged->count(), 0);
  EXPECT_EQ(yChanged->count(), 2);
  EXPECT_EQ(custom_plot->yAxis->range().lower, expected_min);
  EXPECT_EQ(custom_plot->yAxis->range().upper, expected_max);
}

//! Model with two AxisItem's. Controller first is subscribed to one item, then to another.

//! FIXME enable oneControllerTwoAxisItems after  item delete

// TEST_F(ViewportAxisPlotControllerTests, oneControllerTwoAxisItems)
//{
//   auto custom_plot = std::make_unique<QCustomPlot>();

//  // creating the model with single ViewportAxisItem
//  ApplicationModel model;
//  auto axis_item0 = model.InsertItem<ViewportAxisItem>();
//  axis_item0->SetMin(1.0);
//  axis_item0->SetMax(2.0);

//  auto axis_item1 = model.InsertItem<ViewportAxisItem>();
//  axis_item1->SetMin(10.0);
//  axis_item1->SetMax(20.0);

//  // setting up QCustomPlot and item controller.
//  auto controller = std::make_unique<ViewportAxisPlotController>(custom_plot->xAxis);
//  controller->SetItem(axis_item0);
//  auto xChanged = createSpy(custom_plot->xAxis);
//  auto yChanged = createSpy(custom_plot->yAxis);

//  // initial axis status
//  EXPECT_EQ(axis_item0->GetMin(), custom_plot->xAxis->range().lower);
//  EXPECT_EQ(axis_item0->GetMax(), custom_plot->xAxis->range().upper);

//  // switching to second axis
//  controller->SetItem(axis_item1);

//  EXPECT_EQ(xChanged->count(), 1);
//  EXPECT_EQ(yChanged->count(), 0);

//  EXPECT_EQ(axis_item1->GetMin(), custom_plot->xAxis->range().lower);
//  EXPECT_EQ(axis_item1->GetMax(), custom_plot->xAxis->range().upper);

//  // changing QCPAxis
//  const double expected_min = 100.0;
//  const double expected_max = 200.0;
//  custom_plot->xAxis->setRange(expected_min, expected_max);

//  // previous axis should still have original values
//  EXPECT_EQ(axis_item0->GetMin(), 1.0);
//  EXPECT_EQ(axis_item0->GetMax(), 2.0);

//  // second axis should get values from QCPAxis
//  EXPECT_EQ(axis_item1->GetMin(), expected_min);
//  EXPECT_EQ(axis_item1->GetMax(), expected_max);

//  // removing axes from the model
//  model.RemoveItem(axis_item0);
//  model.RemoveItem(axis_item1);
//  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 0);

//  // no UB should follow (valgrind will tell us)
//  custom_plot->xAxis->setRange(1.0, 2.0);

//  // destroying controller, no UB
//  controller.reset();
//  custom_plot->xAxis->setRange(2.0, 3.0);
//}

//! Controller subscribed to ViewportAxisItem.
//! Change ViewportAxisItem title and check that QCPAxis got new values.

TEST_F(ViewportAxisPlotControllerTests, ChangeAxisTitle)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  // creating the model with single ViewportAxisItem
  ApplicationModel model;
  auto axis_item = model.InsertItem<ViewportAxisItem>();

  // setting up QCustomPlot and item controller.
  auto qcp_axis = custom_plot->xAxis;
  ViewportAxisPlotController controller(qcp_axis);
  controller.SetItem(axis_item);

  // changing title
  axis_item->GetTitle()->SetText("abc");

  // no need to change title size and font (checked in axistitlecontroller.test)

  // QCPAxis should switch to logarithmic
  EXPECT_EQ(qcp_axis->label(), QString("abc"));
}
