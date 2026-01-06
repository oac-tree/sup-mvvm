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

#ifndef MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_
#define MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>

#include <QObject>

namespace mvvm
{

/**
 * @brief The LineSeriesMouseListener class listens for mouse press/release events on points of
 * QLineSeries.
 *
 * It reports an index of pressed point via a callback.
 */
class LineSeriesMouseListener : public QObject
{
  Q_OBJECT

public:
  //!< a callback to report a QLineSeries and its clicked point
  using ReportCallback = std::function<void(QLineSeries*, std::int32_t)>;

  /**
   * @brief Constructs a listener for mouse events on the given QLineSeries.
   *
   * @param line_series A pointer to QLineSeries to listen to.
   * @param callback A callback to report the QLineSeries on mouse events.
   * @param parent_object An optional pointer to the parent QObject.
   */
  explicit LineSeriesMouseListener(QLineSeries* line_series, const ReportCallback& callback,
                                   QObject* parent_object = nullptr);

private:
  std::int32_t GetNearestPointIndex(const QPointF& point) const;
  void OnPressed(const QPointF& point);
  void OnReleased(const QPointF& point);

  QLineSeries* m_line_series{nullptr};
  ReportCallback m_callback;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_
