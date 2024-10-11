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

#ifndef MVVM_PLOTTING_PLOT_TYPES_H_
#define MVVM_PLOTTING_PLOT_TYPES_H_

//! @file
//! Collection of plotting specific types.

#include <cstdint>

namespace mvvm
{

/**
 * @brief The CanvasOperationMode enum defines the mode in which the chart view is.
 *
 * This is steered either from the toolbar via signal/slot, or keyboard shortcuts. For the moment it
 * can be either object selection or panning of the viewport (the space bar key is pressed).
 */
enum CanvasOperationMode : std::uint8_t
{
  kSelection,          //!< allows to select graphs and points
  kPan,                //!< allows to pan axes
  kOperationModeCount  //!< total number of operation mode
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_PLOT_TYPES_H_
