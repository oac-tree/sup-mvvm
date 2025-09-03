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
//! COllection of simple types for node editor.

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

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_TYPES_H_
