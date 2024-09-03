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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_GRAPH_PLOT_CONTROLLER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_GRAPH_PLOT_CONTROLLER_H_

#include <mvvm/signals/item_listener.h>
#include <mvvm/view_export.h>

#include <memory>

class QCustomPlot;

namespace mvvm
{

class GraphItem;

//! Establish communication between QCPGraph and GraphItem.
//! Provides update on QCPGraph (data points, line style, color, etc) when GraphItem is changed.
//! QCPGraph is added to QCustomPlot plottables, when controller is created, and removed from
//! plottables when controller is destroyed.

class MVVM_VIEW_EXPORT GraphPlotController : public ItemController<GraphItem>
{
public:
  explicit GraphPlotController(QCustomPlot* plot);
  ~GraphPlotController() override;

protected:
  void Subscribe() override;
  void Unsubscribe() override;

private:
  struct GraphItemControllerImpl;
  std::unique_ptr<GraphItemControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_GRAPH_PLOT_CONTROLLER_H_
