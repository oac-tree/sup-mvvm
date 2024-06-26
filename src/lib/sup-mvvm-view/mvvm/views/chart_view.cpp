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

namespace mvvm
{

ChartView::ChartView(QWidget *parent) : QtCharts::QChartView(parent)
{
  setMouseTracking(true);
  SetOperationMode(kSelection);
}

ChartView::OperationMode ChartView::GetOperationMode() const
{
  return m_operation_mode;
}

void ChartView::SetOperationMode(OperationMode operation_mode)
{
  if (operation_mode == m_operation_mode)
  {
    return;
  }

  m_operation_mode = operation_mode;

  switch (m_operation_mode)
  {
  case kSelection:
    setDragMode(QGraphicsView::NoDrag);
    setInteractive(true);
    emit OperationModeChanged(kSelection);
    break;
  case kPan:
    setInteractive(false);
    setRubberBand(QChartView::NoRubberBand);
    setDragMode(QChartView::ScrollHandDrag);
    emit OperationModeChanged(kPan);
    break;
  default:
    break;
  }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (GetOperationMode() != kPan && !event->isAutoRepeat())
    {
      // user pressed a spacebar, switching operation mode to "pan"
      SetOperationMode(kPan);
      return;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void ChartView::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (GetOperationMode() != kSelection && !event->isAutoRepeat())
    {
      // user released a spacebar, switching operation mode to "selection"
      SetOperationMode(kSelection);
      return;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
  if (m_operation_mode == kPan)
  {
    // user pressed and holding the mouse while in "pan" mode, preparing to scroll canvas
    m_last_pos = event->localPos();
    m_current_activity = kScrollCanvas;
    chart()->setAnimationOptions(QtCharts::QChart::NoAnimation);
  }
  QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
  if (m_current_activity == kScrollCanvas)
  {
    const double dx = -(event->localPos().x() - m_last_pos.x());
    const double dy = event->pos().y() - m_last_pos.y();
    chart()->scroll(dx, dy);
    m_last_pos = event->localPos();
  }

  QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
  m_current_activity = kIdle;
  m_last_pos = {};
  chart()->setAnimationOptions(QtCharts::QChart::AllAnimations);
  QChartView::mouseReleaseEvent(event);
}

}  // namespace sup::gui
