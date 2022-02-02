/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PLOTTING_STATUSSTRINGREPORTERFACTORY_H
#define MVVM_PLOTTING_STATUSSTRINGREPORTERFACTORY_H

//! Contains factory methods to create StatusStringReporter

#include "mvvm/view_export.h"

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

#endif  // MVVM_PLOTTING_STATUSSTRINGREPORTERFACTORY_H
