/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#include "chart_view.h"

#include <mvvm/plotting/charts/qt_charts.h>

namespace mvvm
{

ChartView::ChartView(QWidget *parent) : QChartView(parent)
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
    chart()->setAnimationOptions(m_animation_options_backup);
    setDragMode(QGraphicsView::NoDrag);
    setInteractive(true);
    emit OperationModeChanged(static_cast<int>(CanvasOperationMode::kSelection));
    return;
  }

  if (m_operation_mode == CanvasOperationMode::kPan)
  {
    m_animation_options_backup = chart()->animationOptions();
    chart()->setAnimationOptions(QChart::NoAnimation);

    setInteractive(false);
    setRubberBand(QChartView::NoRubberBand);
    setDragMode(QChartView::ScrollHandDrag);
    emit OperationModeChanged(static_cast<int>(CanvasOperationMode::kPan));
    return;
  }
}

void ChartView::keyPressEvent(QKeyEvent *event)
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

void ChartView::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (GetOperationMode() != CanvasOperationMode::kSelection && !event->isAutoRepeat())
    {
      // user released a spacebar, switching operation mode to "selection"
      SetOperationMode(CanvasOperationMode::kSelection);
      return;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
  if (m_operation_mode == CanvasOperationMode::kPan)
  {
    // user pressed and holding the mouse while in "pan" mode, preparing to scroll canvas

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_last_pos = event->localPos();
#else
    m_last_pos = event->position();
#endif
    m_current_activity = kScrollCanvas;
  }
  QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  const auto event_pos = event->localPos();
#else
  const auto event_pos = event->position();
#endif

  if (m_current_activity == kScrollCanvas)
  {
    const double dx = -(event_pos.x() - m_last_pos.x());
    const double dy = event->pos().y() - m_last_pos.y();
    chart()->scroll(dx, dy);
    m_last_pos = event_pos;
  }

  QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
  m_current_activity = kIdle;
  m_last_pos = {};
  QChartView::mouseReleaseEvent(event);
  // chart()->setAnimationOptions(m_animation_options_backup);
}

}  // namespace mvvm
