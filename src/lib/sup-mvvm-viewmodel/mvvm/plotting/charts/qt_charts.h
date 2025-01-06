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

#ifndef MVVM_PLOTTING_CHARTS_QT_CHARTS_H_
#define MVVM_PLOTTING_CHARTS_QT_CHARTS_H_

//! @file
//! Collection of all important QtCharts headers.
//! The difference between Qt5 and Qt6 syntax is taken into account.

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
using namespace QtCharts;
#else
#include <QAbstractAxis>
#include <QChart>
#include <QChartView>
#include <QLegend>
#include <QLineSeries>
#include <QValueAxis>
#endif

#endif  // MVVM_PLOTTING_CHARTS_QT_CHARTS_H_
