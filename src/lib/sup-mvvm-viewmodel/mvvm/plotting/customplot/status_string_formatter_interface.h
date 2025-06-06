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

#include <mvvm/view_export.h>

#include <string>

class QCustomPlot;

namespace mvvm
{

//! Pure virtual interface to format string with status info corresponding to the current mouse
//! position on QCustomPlot.

class MVVM_VIEW_EXPORT StatusStringFormatterInterface
{
public:
  virtual ~StatusStringFormatterInterface() = default;

  virtual std::string status_string(QCustomPlot* custom_plot, double x, double y) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_STATUS_STRING_FORMATTER_INTERFACE_H_
