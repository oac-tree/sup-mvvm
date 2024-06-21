/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PLOTTING_STATUS_STRING_REPORTER_H_
#define MVVM_PLOTTING_STATUS_STRING_REPORTER_H_

#include <mvvm/view_export.h>

#include <functional>
#include <memory>
#include <string>

class QCustomPlot;

namespace mvvm
{

class StatusStringFormatterInterface;

//! Reports back status string composed for current mouse position in QCustomPlot.
//! Doesn't report if cursor is outside of the axes range.

class MVVM_VIEW_EXPORT StatusStringReporter
{
public:
  using callback_t = std::function<void(const std::string&)>;
  StatusStringReporter(QCustomPlot* custom_plot, callback_t callback,
                       std::unique_ptr<StatusStringFormatterInterface> formatter);
  ~StatusStringReporter();

private:
  struct StatusStringReporterImpl;
  std::unique_ptr<StatusStringReporterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_STATUS_STRING_REPORTER_H_
