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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_REPORTER_FACTORY_H_
#define MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_REPORTER_FACTORY_H_

//! Contains factory methods to create StatusStringReporter

#include <mvvm/view_export.h>

#include <functional>
#include <memory>

class QCustomPlot;

namespace mvvm
{

class StatusStringReporter;

//! Creates reporter for status string in QCustomPlot containing graphs.
MVVM_VIEW_EXPORT std::unique_ptr<StatusStringReporter> CreateGraphReporter(
    QCustomPlot* custom_plot, std::function<void(const std::string&)> callback);

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_REPORTER_FACTORY_H_
