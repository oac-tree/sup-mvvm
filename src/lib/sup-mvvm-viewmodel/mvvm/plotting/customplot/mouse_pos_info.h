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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_MOUSE_POS_INFO_H_
#define MVVM_PLOTTING_CUSTOMPLOT_MOUSE_POS_INFO_H_

#include <mvvm/view_export.h>

class QCustomPlot;

namespace mvvm
{

//! Aggregate to hold mouse position info in QCustomPlot context.
//! Position is given in axis coordinates corresponding to the current zoom level.

struct MVVM_VIEW_EXPORT MousePosInfo
{
  double xpos{0.0};
  double ypos{0.0};
  bool in_axes_range{false};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_MOUSE_POS_INFO_H_
