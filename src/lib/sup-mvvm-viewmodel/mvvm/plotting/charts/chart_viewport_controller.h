/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#ifndef MVVM_PLOTTING_CHARTS_CHART_VIEWPORT_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_CHART_VIEWPORT_CONTROLLER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>
#include <mvvm/signals/item_controller.h>

#include <list>

namespace mvvm
{

class ChartAxisPlotController;
class ChartViewportItem;
class LineSeriesController;
class LineSeriesItem;
class PointItem;

/**
 * @brief The ChartViewportController class establishes communication between ChartViewportItem and
 * QChart scene for plotting multiple line series on one canvas.
 *
 * It listens ChartViewportItems and propagates changes into QChart scene. On insertion/removal of
 * LineSeriesItem it will create/remove corresponding QLineSeries object.
 */
class ChartViewportController : public ItemController<ChartViewportItem>
{
public:
  /**
   * @brief Main constructor.
   * @param chart The Qt's chart to manipulate.
   */
  explicit ChartViewportController(QChart* chart);
  ~ChartViewportController() override;

  ChartViewportController(const ChartViewportController&) = delete;
  ChartViewportController& operator=(const ChartViewportController&) = delete;
  ChartViewportController(ChartViewportController&&) noexcept = delete;
  ChartViewportController& operator=(ChartViewportController&&) noexcept = delete;

  void Subscribe() override;

  QAbstractAxis* GetXQtAxis() const;

  QAbstractAxis* GetYQtAxis() const;

  /**
   * @brief Update the coordinate of the active point.
   *
   * This method is supposed to be called from ChartView when the user moves the mouse while holding
   * a button. It will be propagated to all underlying LineSeriesController objects. They will
   * decide whether there is an active point to move and update its coordinates.
   *
   * This design is because of the peculiarities of the QLineSeries item. It knows when there is a
   * click nearby, but it doesn't know if there is a mouse move.
   */
  void UpdateActivePointCoordinate(double x, double y);

  /**
   * @brief Mark the point as it is selected by the user.
   */
  void SetPointsSelected(const std::vector<PointItem*>& points);

private:
  /**
   * @brief Setup Qt chart for given LineSeriesItem.
   *
   * It will create necessary controller and add QtChart::QLineSeries to the chart.
   */
  QLineSeries* SetupChartForLineSeries(LineSeriesItem* item);

  /**
   * @brief Performs initial setup of QT chart for already existing line series.
   */
  void SetupChart();

  /**
   * @brief Creates Qt chart axes and controllers.
   */
  void SetupAxes();

  /**
   * @brief Process event when new LineSeriesItem added to viewport.
   */
  void OnItemInsertedEvent(const ItemInsertedEvent& event);

  /**
   * @brief Process event when LineSeriesItem is removed from viewport.
   */
  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event);

  /**
   * @brief Process event when one of properties has changed.
   */
  void OnPropertyChangedEvent(const PropertyChangedEvent& event);

  QChart* m_chart{nullptr};
  std::list<std::unique_ptr<LineSeriesController>> m_line_controllers;
  std::unique_ptr<ChartAxisPlotController> m_x_axis_controller;
  std::unique_ptr<ChartAxisPlotController> m_y_axis_controller;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_CHART_VIEWPORT_CONTROLLER_H_
