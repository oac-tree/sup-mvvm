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

#ifndef MVVM_VIEWS_CHART_VIEW_H_
#define MVVM_VIEWS_CHART_VIEW_H_

#include <QtCharts/QChartView>

namespace mvvm
{

/**
 * @brief The ChartView class shows the content of QtCharts::QChart, and provides additional
 * selection functionality to pan axes and select graphs.
 */
class ChartView : public QtCharts::QChartView
{
  Q_OBJECT

public:
  /**
   * @brief The OperationMode enum defines the mode in which the chart view is.
   *
   * @details This is steered either from the toolbar via signal/slot, or keyboard shortcuts.
   * For the moment it can be either object selection or panning of the viewport (the space bar key
   * is pressed).
   */
  enum OperationMode
  {
    kSelection,          //!< allows to select graphs and points
    kPan,                //!< allows to pan axes
    kOperationModeCount  //!< total number of operation mode
  };

  explicit ChartView(QWidget* parent = nullptr);

  OperationMode GetOperationMode() const;

  void SetOperationMode(OperationMode operation_mode);

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

  OperationMode m_operation_mode{kSelection};
  CurrentActivity m_current_activity{kIdle};
  QPointF m_last_pos;
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_CHART_VIEW_H_
