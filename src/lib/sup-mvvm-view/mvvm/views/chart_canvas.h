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

#ifndef MVVM_VIEWS_CHART_CANVAS_H_
#define MVVM_VIEWS_CHART_CANVAS_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>

#include <QWidget>
#include <memory>

namespace mvvm
{

class ChartViewportItem;
class ChartViewportController;
class ChartView;

/**
 * @brief The GraphCanvas class represent widget with embeded QT's chart.
 *
 * It can show multiple graphs following ChartViewportItem settings.
 */
class ChartCanvas : public QWidget
{
  Q_OBJECT

public:
  explicit ChartCanvas(QWidget* parent_widget = nullptr);
  ~ChartCanvas() override;

  void SetViewport(ChartViewportItem* viewport);

  void ZoomIn();

  void ZoomOut();

  void SetViewportToContent();

  void SetOperationMode(int mode);

signals:
  void OperationModeChanged(int mode);

protected:
  void wheelEvent(QWheelEvent* event) override;

private:
  std::unique_ptr<ChartViewportController> m_viewport_controller;

  QChart* m_chart{nullptr};
  ChartView* m_chart_view{nullptr};
  ChartViewportItem* m_viewport{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_CHART_CANVAS_H_
