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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_NODE_PORT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_NODE_PORT_H_

#include <mvvm/nodeeditor/i_node_port.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * The MockNodePort class validates calls to INodePort.
 */
class MockNodePort : public INodePort
{
public:
  MOCK_METHOD(PortDirection, GetPortDirection, (), (const, override));
  MOCK_METHOD(std::string, GetPortType, (), (const, override));
  MOCK_METHOD(std::vector<const INodeConnection*>, GetConnections, (), (const, override));
  MOCK_METHOD(void, AppendConnection, (const INodeConnection*), (override));
  MOCK_METHOD(void, RemoveConnection, (const INodeConnection*), (override));
  MOCK_METHOD(bool, IsConnected, (const INodePort*), (const, override));
  MOCK_METHOD(bool, IsConnectable, (const INodePort*), (const, override));
  MOCK_METHOD((std::pair<double, double>), GetScenePosition, (), (const, override));
  MOCK_METHOD(void, SetScenePosition, (double, double), (override));
  MOCK_METHOD(PortInfo, GetPortInfo, (), (const, override));
  MOCK_METHOD(void, UpdatePortShape, (const NodeOperationStates&), (override));
  MOCK_METHOD(void, SetNotifyShapeCallback,
              (const std::function<void(const NodeOperationStates& operation_states)>&),
              (override));
};

/**
 * @brief The NodePortDecorator class is a simple forward decorator.
 */
class NodePortDecorator : public INodePort
{
public:
  explicit NodePortDecorator(INodePort* decoratee);

  PortDirection GetPortDirection() const override;
  std::string GetPortType() const override;
  std::vector<const INodeConnection*> GetConnections() const override;
  void AppendConnection(const INodeConnection* connection) override;
  void RemoveConnection(const INodeConnection* connection) override;
  bool IsConnected(const INodePort* port) const override;
  bool IsConnectable(const INodePort* port) const override;
  std::pair<double, double> GetScenePosition() const override;
  void SetScenePosition(double xpos, double ypos) override;
  PortInfo GetPortInfo() const override;
  void UpdatePortShape(const NodeOperationStates& operation_states) override;
  void SetNotifyShapeCallback(
      const std::function<void(const NodeOperationStates& operation_states)>& callback) override;

private:
  INodePort* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_NODE_PORT_H_
