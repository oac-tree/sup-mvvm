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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
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
  explicit ChartView(QWidget* parent = nullptr);

  CanvasOperationMode GetOperationMode() const;

  void SetOperationMode(CanvasOperationMode operation_mode);

signals:
  void OperationModeChanged(int mode);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  /**
   * @brief The CurrentActivity enum defines current activity being performed by the user, which
   * requires special awarness of the widget.
   */
  enum CurrentActivity
  {
    kIdle,          //!< no special activity
    kScrollCanvas,  //!< moving mouse in kPan mode, while holding mouse button pressed
    kActivityCount
  };

  CanvasOperationMode m_operation_mode{CanvasOperationMode::kSelection};
  CurrentActivity m_current_activity{kIdle};
  QPointF m_last_pos;
  QChart::AnimationOptions m_animation_options_backup;
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_CHART_VIEW_H_
