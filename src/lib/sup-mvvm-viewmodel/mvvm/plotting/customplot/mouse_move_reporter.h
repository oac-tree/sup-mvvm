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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_MOUSE_MOVE_REPORTER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_MOUSE_MOVE_REPORTER_H_

#include <mvvm/view_export.h>

#include <functional>
#include <memory>

class QCustomPlot;

namespace mvvm
{

struct MousePosInfo;

//! Tracks mouse moves in QCustomPlot canvas.
//! Notifies client about mouse moves and corresponding pointer coordinates expressed in axes units
//! at current zoom level.

class MVVM_VIEW_EXPORT MouseMoveReporter
{
public:
  using callback_t = std::function<void(const MousePosInfo& pos_info)>;
  MouseMoveReporter(QCustomPlot* custom_plot, callback_t callback);
  ~MouseMoveReporter();

private:
  struct MouseMoveReporterImpl;
  std::unique_ptr<MouseMoveReporterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_MOUSE_MOVE_REPORTER_H_
