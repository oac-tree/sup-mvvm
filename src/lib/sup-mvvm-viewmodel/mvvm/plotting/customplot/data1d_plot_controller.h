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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_DATA1D_PLOT_CONTROLLER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_DATA1D_PLOT_CONTROLLER_H_

#include <mvvm/signals/item_listener.h>
#include <mvvm/view_export.h>

#include <memory>

class QCPGraph;

namespace mvvm
{

class Data1DItem;

//! Establishes communication between QCPGraph and Data1DItem.
//! Provides update of data points on QCPGraph when Graph1DItem is changed.

class MVVM_VIEW_EXPORT Data1DPlotController : public ItemController<Data1DItem>
{
public:
  explicit Data1DPlotController(QCPGraph* graph);
  ~Data1DPlotController() override;

protected:
  void Subscribe() override;
  void Unsubscribe() override;

private:
  struct Data1DPlotControllerImpl;
  std::unique_ptr<Data1DPlotControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_DATA1D_PLOT_CONTROLLER_H_
