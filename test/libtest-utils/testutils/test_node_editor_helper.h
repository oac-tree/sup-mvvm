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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_

//! @file
//! Collection of helper functions to test QGraphicsScene and NodeEditor.

#include <mvvm/nodeeditor/node_editor_types.h>

#include <memory>

namespace mvvm
{
class NodePortShape;
}

namespace mvvm::test
{

/**
 * @brief Creates node port shape for given direction.
 */
std::unique_ptr<NodePortShape> CreateNodePortShape(
    PortDirection port_direction = PortDirection::kInput);

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_
