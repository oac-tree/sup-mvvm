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

#ifndef MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>
#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{

class LineSeriesDataItem;
class ModelListener;

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

  void SetItem(const LineSeriesDataItem* item);

  void OnModelEvent(const ItemInsertedEvent& event);

  void OnModelEvent(const AboutToRemoveItemEvent& event);

  /**
   * @brief Propagates change of (x,y) values to QtCharts.
   */
  void OnModelEvent(const DataChangedEvent& event);

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

private:
  void Subscribe();
  void Unsubscribe();

  /**
   * @brief Provide initial setup of QLineSeries from
   */
  void InitLineSeriesData();

  QLineSeries* m_qt_line_series{nullptr};
  const LineSeriesDataItem* m_data_item{nullptr};
  std::unique_ptr<ModelListener> m_listener;
  double m_x_offset{0.0};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_LINE_SERIES_DATA_CONTROLLER_H_
