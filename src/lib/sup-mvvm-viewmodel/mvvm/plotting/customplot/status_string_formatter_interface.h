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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_FORMATTER_INTERFACE_H_
#define MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_FORMATTER_INTERFACE_H_

#include <string>

class QCustomPlot;

namespace mvvm
{

//! Pure virtual interface to format string with status info corresponding to the current mouse
//! position on QCustomPlot.

class StatusStringFormatterInterface
{
public:
  StatusStringFormatterInterface() = default;
  virtual ~StatusStringFormatterInterface() = default;

  StatusStringFormatterInterface(const StatusStringFormatterInterface&) = delete;
  StatusStringFormatterInterface& operator=(const StatusStringFormatterInterface&) = delete;
  StatusStringFormatterInterface(StatusStringFormatterInterface&&) = delete;
  StatusStringFormatterInterface& operator=(StatusStringFormatterInterface&&) = delete;

  virtual std::string status_string(QCustomPlot* custom_plot, double xpos, double ypos) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_FORMATTER_INTERFACE_H_
