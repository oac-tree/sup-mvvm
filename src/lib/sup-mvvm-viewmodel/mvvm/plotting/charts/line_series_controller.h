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

#ifndef MVVM_PLOTTING_CHARTS_LINE_SERIES_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_LINE_SERIES_CONTROLLER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>
#include <mvvm/signals/item_controller.h>

#include <memory>

namespace mvvm
{

class LineSeriesItem;
class LineSeriesDataController;
class ChartPenController;

/**
 * @brief The LineSeriesDataController class establishes communication between LineSeriesItem object
 * representing waveform data and QLineSeries.
 *
 * For the moment controller only takes care of proper linking to external data. The most
 * of the work (adding/removal data points) is carried by underlying LineSeriesDataController.
 *
 * In the future it will be extended to control generic graph  properties, like a pen, titles, etc.
 */
class LineSeriesController : public mvvm::ItemController<LineSeriesItem>
{
public:
  /**
   * @brief Main constructor.
   *
   * @param line_series The Qt Line series object to control.
   */
  explicit LineSeriesController(QLineSeries* line_series);
  ~LineSeriesController() override;

  void Subscribe() override;

  /**
   * @brief Return Qt series under control.
   */
  QLineSeries* GetQtLineSeries() const;

private:
  /**
   * @brief Processes the change of linked data item.
   */
  void OnPropertyChanged(const PropertyChangedEvent& event);

  /**
   * @brief Processes the change of the display name.
   */
  void OnDataChanged(const DataChangedEvent& event);

  std::unique_ptr<LineSeriesDataController> m_data_controller;
  std::unique_ptr<ChartPenController> m_pen_controller;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_LINE_SERIES_CONTROLLER_H_
