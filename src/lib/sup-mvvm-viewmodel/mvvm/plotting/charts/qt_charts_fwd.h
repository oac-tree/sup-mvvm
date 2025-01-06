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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PLOTTING_CHARTS_QT_CHARTS_FWD_H_
#define MVVM_PLOTTING_CHARTS_QT_CHARTS_FWD_H_

//! @file
//! Forward declaration of main QtCharts objects.
//! The difference between Qt5 and Qt6 is taken into account.

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace QtCharts
{
class QAbstractAxis;
class QLineSeries;
class QChart;
class QChartView;
class QValueAxis;
}  // namespace QtCharts
using namespace QtCharts;
#else
class QAbstractAxis;
class QLineSeries;
class QChart;
class QChartView;
class QValueAxis;
#endif

#endif  // MVVM_PLOTTING_CHARTS_QT_CHARTS_FWD_H_
