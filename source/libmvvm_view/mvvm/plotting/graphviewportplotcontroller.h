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

#ifndef MVVM_PLOTTING_GRAPHVIEWPORTPLOTCONTROLLER_H
#define MVVM_PLOTTING_GRAPHVIEWPORTPLOTCONTROLLER_H

#include "mvvm/signals/itemlistener.h"
#include "mvvm/view_export.h"

#include <memory>

class QCustomPlot;

namespace mvvm
{

class GraphViewportItem;

//! Establishes communications and mutual updates for GraphViewportItem and QCutomPlot.
//! Populates custom plot with all graphs found in GraphViewportItem.

class MVVM_VIEW_EXPORT GraphViewportPlotController : public ItemListener<GraphViewportItem>
{
public:
  explicit GraphViewportPlotController(QCustomPlot* plot);
  ~GraphViewportPlotController() override;

protected:
  void Subscribe() override;

private:
  struct GraphViewportPlotControllerImpl;
  std::unique_ptr<GraphViewportPlotControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_GRAPHVIEWPORTPLOTCONTROLLER_H
