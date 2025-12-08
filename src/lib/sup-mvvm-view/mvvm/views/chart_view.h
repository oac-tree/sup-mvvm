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

#ifndef MVVM_VIEWS_CHART_VIEW_H_
#define MVVM_VIEWS_CHART_VIEW_H_

#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/plotting/plot_types.h>

namespace mvvm
{

/**
 * @brief The ChartView class shows the content of QChart, and provides additional
 * selection functionality to pan axes and select graphs.
 */
class ChartView : public QChartView
{
  Q_OBJECT

public:
  explicit ChartView(QChart* chart_object, QWidget* parent_widget = nullptr);

  CanvasOperationMode GetOperationMode() const;

  /**
   * @brief Sets operation mode of chart view.
   */
  void SetOperationMode(CanvasOperationMode operation_mode);

  /**
   * @brief Sets current activity being performed by the user.
   */
  void SetCanvasCurrentActivity(CanvasCurrentActivity activity);

  CanvasCurrentActivity GetCanvasCurrentActivity() const;

signals:
  void OperationModeChanged(int mode);
  void MovePointRequested(const QPointF& event_position);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  CanvasOperationMode m_operation_mode{CanvasOperationMode::kSelection};
  CanvasCurrentActivity m_current_activity{CanvasCurrentActivity::kIdle};
  QPointF m_last_pos;
  QChart::AnimationOptions m_animation_options_backup;
  bool m_mouse_pressed{false};
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_CHART_VIEW_H_
