/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/status_string_reporter_factory.h"

#include "mvvm/plotting/graph_info_formatter.h"
#include "mvvm/plotting/status_string_reporter.h"

namespace mvvm
{

std::unique_ptr<StatusStringReporter> CreateGraphReporter(
    QCustomPlot* custom_plot, std::function<void(const std::string&)> callback)
{
  return std::make_unique<StatusStringReporter>(custom_plot, callback,
                                                std::make_unique<GraphInfoFormatter>());
}

}  // namespace mvvm
