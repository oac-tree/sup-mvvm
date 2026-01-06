/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_VIEWPORT_AXIS_PLOT_CONTROLLER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_VIEWPORT_AXIS_PLOT_CONTROLLER_H_

#include <mvvm/signals/item_controller.h>

#include <memory>

class QCPAxis;

namespace mvvm
{

class ViewportAxisItem;

//! Establishes communication between QCPAxis and ViewportAxisItem.
//! Provide mutual update of axis parameters (min, max, title) for two axes representations.

class ViewportAxisPlotController : public ItemController<ViewportAxisItem>
{
public:
  explicit ViewportAxisPlotController(QCPAxis* axis);
  ~ViewportAxisPlotController() override;

  ViewportAxisPlotController(const ViewportAxisPlotController&) = delete;
  ViewportAxisPlotController& operator=(const ViewportAxisPlotController&) = delete;
  ViewportAxisPlotController(ViewportAxisPlotController&&) noexcept = delete;
  ViewportAxisPlotController& operator=(ViewportAxisPlotController&&) noexcept = delete;

protected:
  void Subscribe() override;
  void Unsubscribe() override;

public:
  struct AxesPlotControllerImpl;
  std::unique_ptr<AxesPlotControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_VIEWPORT_AXIS_PLOT_CONTROLLER_H_
