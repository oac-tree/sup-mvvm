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

#ifndef MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_
#define MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_

//! @file
//! Collection of default constants for NodeEditor. Some constants a wrapped to function calls to
//! make MISRA happy.

#include <QFont>
#include <QRectF>

namespace mvvm::constants
{

/**
 * @brief Returns default radious of the port.
 */
double GetDefaultPortRadius();

/**
 * @brief Returns default font for ConnectableShape label.
 */
QFont GetDefaultConnectableShapeFont();

/**
 * @brief Returns default font for NodePort label.
 */
QFont GetDefaultPortLabelFont();

/**
 * @brief Returns default rectangle for connectable shape.
 */
QRectF GetDefaultConnectableShapeRect();

}  // namespace mvvm::constants

#endif  // MVVM_NODEEDITOR_NODE_EDITOR_CONSTANTS_H_
