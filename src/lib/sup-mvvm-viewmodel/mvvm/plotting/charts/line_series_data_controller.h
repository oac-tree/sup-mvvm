/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>
#include <mvvm/signals/event_types.h>

#include <memory>
#include <vector>

namespace mvvm
{

class LineSeriesDataItem;
class ModelListener;
class LineSeriesMouseListener;
class PointItem;

/**
 * @brief The LineSeriesDataController class establishes communication between LineSeriesDataItem
 * object representing waveform data and QLineSeries.
 *
 * For the moment it is one way communication from LineSeriesDataItem toward QLineSeries. Any
 * change in LineSeriesDataItem (adding, removing data points, changing x,y values) will be
 * propagated to QLineSeries.
 */
class LineSeriesDataController
{
public:
  /**
   * @brief Main constructor.
   *
   *  @param line_series The Qt Line series object to control.
   */
  explicit LineSeriesDataController(QLineSeries* line_series);
  ~LineSeriesDataController();

  LineSeriesDataController(const LineSeriesDataController&) = delete;
  LineSeriesDataController& operator=(const LineSeriesDataController&) = delete;
  LineSeriesDataController(LineSeriesDataController&&) noexcept = delete;
  LineSeriesDataController& operator=(LineSeriesDataController&&) noexcept = delete;

  void SetItem(const LineSeriesDataItem* item);

  void OnItemInsertedEvent(const ItemInsertedEvent& event);

  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event);

  /**
   * @brief Propagates change of (x,y) values to QtCharts.
   */
  void OnDataChangedEvent(const DataChangedEvent& event);

  const LineSeriesDataItem* GetDataItem() const;

  /**
   * @brief Return Qt series under control.
   */
  QLineSeries* GetQtLineSeries() const;

  /**
   * @brief Sets the value of x-axis offset.
   *
   * This number will be added to x-coordinates of all points.
   */
  void SetXOffset(double value);

  /**
   * @brief Returns the currently active point being moved by user, or nullptr if no point is
   * active.
   */
  PointItem* GetActivePoint() const;

  /**
   * @brief Mark the point as it is selected by the user.
   */
  void SetSelectedPoints(const std::vector<PointItem*>& points);

private:
  void Subscribe();
  void Unsubscribe();
  std::unique_ptr<LineSeriesMouseListener> CreateMouseListener();

  /**
   * @brief Provide initial setup of QLineSeries from
   */
  void InitLineSeriesData();

  QLineSeries* m_qt_line_series{nullptr};
  const LineSeriesDataItem* m_data_item{nullptr};
  std::unique_ptr<ModelListener> m_listener;
  double m_x_offset{0.0};
  std::unique_ptr<LineSeriesMouseListener> m_mouse_listener;
  PointItem* m_active_point{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_
