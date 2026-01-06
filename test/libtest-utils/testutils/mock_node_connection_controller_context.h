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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_GUIDE_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_GUIDE_CONTEXT_H_

#include <mvvm/nodeeditor/node_connection_guide_context.h>
#include <mvvm/nodeeditor/node_editor_types.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockNodeConnectionGuideContext listens for callbacks defined in
 * NodeConnectionGuideContext and it is used in testing of NodeConnectionGuideImpl.
 */
class MockNodeConnectionGuideContext
{
public:
  MOCK_METHOD(void, OnConnectionStarted, (), ());
  MOCK_METHOD(void, OnConnectionFinished, (), ());
  MOCK_METHOD(void, OnConnectionRequest, (const INodePort*, const INodePort*), ());

  NodeConnectionGuideContext CreateContext();
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_GUIDE_CONTEXT_H_
