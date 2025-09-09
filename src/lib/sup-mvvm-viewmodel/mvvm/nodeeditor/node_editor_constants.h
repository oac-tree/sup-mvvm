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

#ifndef MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_
#define MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_

//! @file
//! Collection of default constants for NodeEditor. Some constants a wrapped to function calls to
//! make MISRA happy.

#include <QColor>
#include <QFont>
#include <QGradient>
#include <QRectF>

namespace mvvm::constants
{

/**
 * @brief Characteristic size of node unit.
 *
 * This corresponds to the width of the letter 'M'.
 * FIXME remove duplication after widget_utils.h refactoring.
 */
double NodeUnitSize(double scale);

/**
 * @brief Returns default radious of the port.
 */
double GetDefaultPortRadius();

/**
 * @brief Returns default color of the port.
 */
const QColor& DefaultPortColor();

/**
 * @brief Returns default font for NodePort label.
 */
const QFont& DefaultPortLabelFont();

/**
 * @brief Returns default rectangle for connectable shape.
 */
const QRectF& DefaultConnectableShapeRect();

/**
 * @brief Returns a gradient for connectable shape background.
 */
const QGradient& ConnectableShapeGradient(const QColor& color, const QRectF& rect);

}  // namespace mvvm::constants

#endif  // MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_
