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

#include "mvvm/views/chart_view.h"

#include <mvvm/plotting/charts/qt_charts.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QKeyEvent>
#include <QSignalSpy>

namespace mvvm::test
{

class ChartViewTest : public ::testing::Test
{
public:
  static void SimulateMousePress(QGraphicsView* view, QPointF scene_pos,
                                 Qt::MouseButton button = Qt::LeftButton)
  {
    const QPointF view_pos = view->mapFromScene(scene_pos);
    QMouseEvent press_event(QEvent::MouseButtonPress, view_pos.toPoint(),
                            view->mapToGlobal(view_pos.toPoint()), button, button, Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &press_event);
  }

  static void SimulateMouseMove(QGraphicsView* view, QPointF scene_pos)
  {
    const QPointF view_pos = view->mapFromScene(scene_pos);
    QMouseEvent move_event(QEvent::MouseMove, view_pos.toPoint(),
                           view->mapToGlobal(view_pos.toPoint()), Qt::NoButton, Qt::LeftButton,
                           Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &move_event);
  }

  static void SimulateMouseRelease(QGraphicsView* view, QPointF scene_pos,
                                   Qt::MouseButton button = Qt::LeftButton)
  {
    const QPointF view_pos = view->mapFromScene(scene_pos);
    QMouseEvent release_event(QEvent::MouseButtonRelease, view_pos.toPoint(),
                              view->mapToGlobal(view_pos.toPoint()), button, Qt::NoButton,
                              Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &release_event);
  }
};

TEST_F(ChartViewTest, InitialState)
{
  auto chart = std::make_unique<QChart>();
  auto chart_ptr = chart.get();
  const auto initial_options = chart_ptr->animationOptions();

  const ChartView chart_view(chart.release());
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);

  // animations options are not chainged
  EXPECT_EQ(chart_ptr->animationOptions(), initial_options);
}

TEST_F(ChartViewTest, SetOperationMode)
{
  auto chart = std::make_unique<QChart>();
  chart->setAnimationOptions(QChart::AllAnimations);
  auto chart_ptr = chart.get();

  ChartView chart_view(chart.release());
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);

  QSignalSpy selection_mode_spy(&chart_view, &ChartView::OperationModeChanged);
  chart_view.SetOperationMode(CanvasOperationMode::kPan);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kPan);
  EXPECT_EQ(selection_mode_spy.count(), 1);
  EXPECT_EQ(selection_mode_spy.takeFirst().at(0).toInt(),
            static_cast<int>(CanvasOperationMode::kPan));

  chart_view.SetOperationMode(CanvasOperationMode::kSelection);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);
  EXPECT_EQ(selection_mode_spy.count(), 1);
  EXPECT_EQ(selection_mode_spy.takeFirst().at(0).toInt(),
            static_cast<int>(CanvasOperationMode::kSelection));
}

TEST_F(ChartViewTest, PressAndReleaseSpacebar)
{
  auto chart = std::make_unique<QChart>();
  chart->setAnimationOptions(QChart::AllAnimations);
  auto chart_ptr = chart.get();

  const ChartView chart_view(chart.release());
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);

  QKeyEvent press_event(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
  QApplication::sendEvent(chart_view.viewport(), &press_event);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kPan);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);

  QKeyEvent release_event(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
  QApplication::sendEvent(chart_view.viewport(), &release_event);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);
}

TEST_F(ChartViewTest, PressSpacebarClickMouseAndMove)
{
  auto chart = std::make_unique<QChart>();
  chart->setAnimationOptions(QChart::AllAnimations);
  auto chart_ptr = chart.get();

  ChartView chart_view(chart.release());
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);

  // press spacebar
  QKeyEvent press_event(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
  QApplication::sendEvent(chart_view.viewport(), &press_event);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kPan);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);

  // press mouse button
  const QPointF scene_press_coordinate{10, 10};
  SimulateMousePress(&chart_view, scene_press_coordinate, Qt::LeftButton);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kIdle);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);

  // move mouse
  const QPointF scene_move1_pos{100, 200};
  SimulateMouseMove(&chart_view, scene_move1_pos);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kScrollCanvas);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::NoAnimation);

  // release mouse button
  SimulateMouseRelease(&chart_view, scene_move1_pos, Qt::LeftButton);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kIdle);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);

  // release spacebar
  QKeyEvent release_event(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
  QApplication::sendEvent(chart_view.viewport(), &release_event);
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);
}

TEST_F(ChartViewTest, PressButtonAndMoveMouse)
{
  auto chart = std::make_unique<QChart>();
  chart->setAnimationOptions(QChart::AllAnimations);
  auto chart_ptr = chart.get();
  chart_ptr->createDefaultAxes();

  ChartView chart_view(chart.release());
  EXPECT_EQ(chart_view.GetOperationMode(), CanvasOperationMode::kSelection);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kIdle);
  chart_view.setFocus();
  chart_view.setSceneRect(0, 0, 800, 600);

  QSignalSpy spy_move_point(&chart_view, &ChartView::MovePointRequested);

  const QPointF scene_press_coordinate{10, 10};
  const QPointF scene_move1_pos{100, 200};

  SimulateMousePress(&chart_view, scene_press_coordinate, Qt::LeftButton);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kIdle);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);

  SimulateMouseMove(&chart_view, scene_move1_pos);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kMovePoint);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::NoAnimation);

  EXPECT_EQ(spy_move_point.count(), 1);
  const QPointF reported_position = spy_move_point.takeFirst().at(0).toPointF();
  EXPECT_EQ(reported_position, scene_move1_pos);

  SimulateMouseRelease(&chart_view, scene_move1_pos, Qt::LeftButton);
  EXPECT_EQ(chart_view.GetCanvasCurrentActivity(), CanvasCurrentActivity::kIdle);
  EXPECT_EQ(chart_ptr->animationOptions(), QChart::AllAnimations);
}

}  // namespace mvvm::test
