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

#ifndef MVVM_PLOTTING_CHARTS_CHART_PEN_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_CHART_PEN_CONTROLLER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>
#include <mvvm/signals/item_controller.h>

namespace mvvm
{

class PenItem;

/**
 * @brief The ChartPenController class propagates color, line style and width changes from PenItem
 * to QtChart::QLineSeries.
 */
class ChartPenController : public mvvm::ItemController<mvvm::PenItem>
{
public:
  explicit ChartPenController(QLineSeries* line_series);

  /**
   * @brief Return Qt series under control.
   */
  QLineSeries* GetQtLineSeries() const;

protected:
  void Subscribe() override;

private:
  /**
   * @brief Processes the change of properties.
   */
  void OnPropertyChanged(const mvvm::PropertyChangedEvent& event);

  /**
   * @brief Update line series from item.
   */
  void UpdateLineSeriesFromItem();

  QLineSeries* m_qt_line_series{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_CHART_PEN_CONTROLLER_H_
