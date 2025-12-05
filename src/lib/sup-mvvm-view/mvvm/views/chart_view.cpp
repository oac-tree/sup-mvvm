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

#include "chart_view.h"

#include <mvvm/plotting/charts/qt_charts.h>

namespace mvvm
{

ChartView::ChartView(QChart* chart_object, QWidget* parent_widget)
    : QChartView(chart_object, parent_widget)
{
  setMouseTracking(true);
  SetOperationMode(CanvasOperationMode::kSelection);
  m_animation_options_backup = chart()->animationOptions();
}

CanvasOperationMode ChartView::GetOperationMode() const
{
  return m_operation_mode;
}

void ChartView::SetOperationMode(CanvasOperationMode operation_mode)
{
  if (operation_mode == m_operation_mode)
  {
    return;
  }

  m_operation_mode = operation_mode;

  if (m_operation_mode == CanvasOperationMode::kSelection)
  {
    setDragMode(QGraphicsView::NoDrag);
    setInteractive(true);
    emit OperationModeChanged(static_cast<int>(CanvasOperationMode::kSelection));
    return;
  }

  if (m_operation_mode == CanvasOperationMode::kPan)
  {
    setInteractive(false);
    setRubberBand(QChartView::NoRubberBand);
    setDragMode(QChartView::ScrollHandDrag);
    emit OperationModeChanged(static_cast<int>(CanvasOperationMode::kPan));
    return;
  }
}

void ChartView::SetCanvasCurrentActivity(CanvasCurrentActivity activity)
{
  m_current_activity = activity;

  if (m_current_activity == CanvasCurrentActivity::kIdle)
  {
    chart()->setAnimationOptions(m_animation_options_backup);
  }

  if ((m_current_activity == CanvasCurrentActivity::kScrollCanvas)
      || (m_current_activity == CanvasCurrentActivity::kMovePoint))
  {
    m_animation_options_backup = chart()->animationOptions();
    chart()->setAnimationOptions(QChart::NoAnimation);
  }
}

CanvasCurrentActivity ChartView::GetCanvasCurrentActivity() const
{
  return m_current_activity;
}

void ChartView::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (GetOperationMode() != CanvasOperationMode::kPan && !event->isAutoRepeat())
    {
      // user pressed a spacebar, switching operation mode to "pan"
      SetOperationMode(CanvasOperationMode::kPan);
      return;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void ChartView::keyReleaseEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (GetOperationMode() == CanvasOperationMode::kPan && !event->isAutoRepeat())
    {
      // user released a spacebar, switching operation mode to "selection"
      SetOperationMode(CanvasOperationMode::kSelection);
      return;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent* event)
{
  if (m_operation_mode == CanvasOperationMode::kPan)
  {
    // user pressed and holding the mouse key while in "pan" mode, preparing to scroll canvas

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_last_pos = event->localPos();
#else
    m_last_pos = event->position();
#endif
    SetCanvasCurrentActivity(CanvasCurrentActivity::kScrollCanvas);
  }
  else
  {
    SetCanvasCurrentActivity(CanvasCurrentActivity::kMovePoint);
  }

  QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent* event)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  const auto event_pos = event->localPos();
#else
  const auto event_pos = event->position();
#endif

  if (m_current_activity == CanvasCurrentActivity::kScrollCanvas)
  {
    const double dx = -(event_pos.x() - m_last_pos.x());
    const double dy = event->pos().y() - m_last_pos.y();
    chart()->scroll(dx, dy);
    m_last_pos = event_pos;
  }

  if (m_current_activity == CanvasCurrentActivity::kMovePoint)
  {
    emit MovePointRequested(event_pos);
  }

  QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent* event)
{
  SetCanvasCurrentActivity(CanvasCurrentActivity::kIdle);
  m_last_pos = {};
  QChartView::mouseReleaseEvent(event);
}

}  // namespace mvvm
