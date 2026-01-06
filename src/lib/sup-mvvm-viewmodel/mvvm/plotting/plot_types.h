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

#ifndef MVVM_PLOTTING_PLOT_TYPES_H_
#define MVVM_PLOTTING_PLOT_TYPES_H_

//! @file
//! Collection of plotting specific types.

#include <cstdint>

namespace mvvm
{

/**
 * @brief The CanvasOperationMode enum defines the mode in which the QGraphicsView is.
 *
 * There are only two modes for the moment. In "selection" mode, the user can click on objects
 * to select/deselect them. In "panning" mode, the user grabs a viewport and moves it to another
 * place (aka canvas scrolling).
 *
 * This is steered either from the toolbar or by pressing/releasing the spacebar key.
 */
enum class CanvasOperationMode : std::uint8_t
{
  kSelection,  //!< allows to select graphs and points
  kPan         //!< allows to pan axes
};

/**
 * @brief The CanvasCurrentActivity enum defines current activity being performed by the user.
 *
 * This is used to distinguish between simple mouse move events and those which are part of
 * some activity like panning the canvas or moving a point of a graph.
 */
enum class CanvasCurrentActivity : std::uint8_t
{
  kIdle,          //!< no special activity
  kScrollCanvas,  //!< moving mouse in kPan mode, while holding mouse button pressed
  kMovePoint,     //!< moving a point of a graph
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_PLOT_TYPES_H_
