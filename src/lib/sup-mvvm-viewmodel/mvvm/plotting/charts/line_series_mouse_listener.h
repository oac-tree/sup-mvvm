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

#ifndef MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_
#define MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_

#include <mvvm/plotting/charts/qt_charts_fwd.h>

#include <QObject>

namespace mvvm
{

/**
 * @brief The LineSeriesInteractionController class provides an information about mouse clicks and
 * hover events on points of QLineSeries.
 */
class LineSeriesMouseListener : public QObject
{
  Q_OBJECT

public:
  explicit LineSeriesMouseListener(QLineSeries* line_series, QObject* parent_object = nullptr);

private:
  QLineSeries* m_line_series{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_LINE_SERIES_MOUSE_LISTENER_H_
