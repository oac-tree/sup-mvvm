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

#ifndef MVVM_VIEWS_CHART_CANVAS_H_
#define MVVM_VIEWS_CHART_CANVAS_H_

#include <QWidget>
#include <memory>

namespace QtCharts
{
class QChart;
}  // namespace QtCharts

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
  explicit ChartCanvas(QWidget* parent = nullptr);
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

  QtCharts::QChart* m_chart{nullptr};
  ChartView* m_chart_view{nullptr};
  ChartViewportItem* m_viewport{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_CHART_CANVAS_H_
