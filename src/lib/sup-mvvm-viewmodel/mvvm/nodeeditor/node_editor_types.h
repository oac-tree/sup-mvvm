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

#ifndef MVVM_NODEEDITOR_NODE_TYPES_H_
#define MVVM_NODEEDITOR_NODE_TYPES_H_

//! @file
//! Collection of simple types for node editor.

#include <mvvm/core/flags.h>

#include <cstdint>

namespace mvvm
{

/**
 * @brief The PortDirection enum introduces port's direction types so we know what can be connected
 * with what.
 *
 * Example: for behavioral tree with top-to-bottom layout
 * Consider parent "Sequence" instruction with a child "Wait" beneath
 *   kInput port is located on a Sequence instruction at its bottom.
 *   kOutput port is located on a Wait instruction at its top.
 */
enum class PortDirection : std::uint8_t
{
  kDefault,
  kInput,
  kOutput
};

/**
 * @brief The PortLocation enum defines where a port is located on a shape.
 *
 * Ports will be evenly distributed along the specified side of the shape.
 */
enum class PortLocation : std::uint8_t
{
  kLeft,
  kTop,
  kRight,
  kBottom,
  kUnknown
};

/**
 * @brief The NodeTreeLayout enum defines presentation of the node tree.
 * It can be either top-to-bottom (like a behavior tree) or left-to-right (like a data flow).
 */
enum class NodeTreeLayout : std::uint8_t
{
  kTopToBottom,
  kLeftToRight
};

/**
 * @brief The GraphicsViewOperationMode enum defines how user interacts with the graphics view.
 */
enum class GraphicsViewOperationMode : std::uint8_t
{
  kSimpleSelection,  // point and click
  kRubberSelection,  // holding the mouse and dragging a rectangle
  kHandDrag,         // click and drag to pan the view
  kUnknownMode
};

/**
 * @brief The EventProcessStatus enum defines possible results of QEvent processing.
 */
enum class EventProcessStatus : std::uint8_t
{
  kStatusUnknown,
  kEventWasHandled,
  kEventWasIgnored
};

/**
 * @brief The NodeOperationState enum defines possible operation states of a node.
 *
 * These are the states which can affect the visual representation of the node: execution state,
 * children state, etc.
 */
enum class NodeOperationState : std::uint8_t
{
  kNotStarted,         //!< node execution not started yet
  kNotFinished,        //!< started and waiting for next tick
  kRunning,            //!< started and running (child) asyncrhonisoulsy
  kSuccess,            //!< finished successfully
  kFailure,            //!< finished unsuccessfully
  kCollapsedChildren,  //!< node has children, and all children are collapsed (hidden)
  size                 //!< obligatory last element
};

using NodeOperationStates = Flags<NodeOperationState>;

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_TYPES_H_
