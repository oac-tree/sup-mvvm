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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_H_

#include <mvvm/nodeeditor/i_node_connection.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * The MockNodeConnection class validates calls to INodeConnection.
 */
class MockNodeConnection : public INodeConnection
{
public:
  MOCK_METHOD(INodePort*, GetStartPort, (), (const, override));
  MOCK_METHOD(void, SetStartPort, (INodePort * port), (override));
  MOCK_METHOD(INodePort*, GetEndPort, (), (const, override));
  MOCK_METHOD(void, SetEndPort, (INodePort * port), (override));
  MOCK_METHOD(bool, HasPort, (const INodePort* port), (const, override));
  MOCK_METHOD(void, ForgetPort, (INodePort * port), (override));
  MOCK_METHOD(void, UpdateConnectionShape, (const NodeOperationStates&), (override));
  MOCK_METHOD(void, SetNotifyShapeCallback, (shape_callback_t), (override));
};

/**
 * @brief The NodeConnectionDecorator class is a simple forward decorator to an INodeConnection.
 */
class NodeConnectionDecorator : public INodeConnection
{
public:
  explicit NodeConnectionDecorator(INodeConnection* decoratee);

  const INodePort* GetStartPort() const override;
  void SetStartPort(INodePort* port) override;
  const INodePort* GetEndPort() const override;
  void SetEndPort(INodePort* port) override;
  bool HasPort(const INodePort* port) const override;
  void ForgetPort(INodePort* port) override;
  void UpdateConnectionShape(const NodeOperationStates& state) override;
  void SetNotifyShapeCallback(shape_callback_t callback) override;

private:
  INodeConnection* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_NODE_CONNECTION_H_
