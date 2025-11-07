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

#ifndef MVVM_NODEEDITOR_NODE_EDITOR_EVENTS_H_
#define MVVM_NODEEDITOR_NODE_EDITOR_EVENTS_H_

//! @file
//! Collection of simple types for node editor.

#include <variant>

namespace mvvm
{

class INodePort;

/**
 * @brief The DoubleClickEvent class represents an event when the user double-clicks on the node
 * port shape.
 */
struct DoubleClickEvent
{
  INodePort* port{nullptr};  //! the model being destroyed
};

bool operator==(const DoubleClickEvent& lhs, const DoubleClickEvent& rhs);
bool operator!=(const DoubleClickEvent& lhs, const DoubleClickEvent& rhs);

using node_event_t = std::variant<DoubleClickEvent>;

}  // namespace mvvm

#endif  //
