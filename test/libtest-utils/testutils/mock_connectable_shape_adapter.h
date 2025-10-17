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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_CONNECTABLE_SHAPE_ADAPTER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_CONNECTABLE_SHAPE_ADAPTER_H_

#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>

#include <gmock/gmock.h>

namespace mvvm
{
class SessionItem;
}

namespace mvvm::test
{

/**
 * @brief The MockConnectableShapeAdapter alows to validate calls to IConnectableShapeAdapter.
 */
class MockConnectableShapeAdapter : public IConnectableShapeAdapter
{
public:
  MOCK_METHOD(mvvm::SessionItem*, GetItem, (), (const, override));
  MOCK_METHOD(QString, GetDisplayName, (), (const, override));
  MOCK_METHOD(double, GetX, (), (const, override));
  MOCK_METHOD(void, SetX, (double), (override));
  MOCK_METHOD(double, GetY, (), (const, override));
  MOCK_METHOD(void, SetY, (double), (override));
  MOCK_METHOD(std::vector<PortInfo>, GetPortInfos, (), (const, override));
  MOCK_METHOD(NodeOperationStates, GetOperationStates, (), (const, override));
  MOCK_METHOD(void, ProcessEvent, (const node_event_t& event), (const, override));
  MOCK_METHOD(ConnectableShapeStyle, GetStyle, (), (const, override));
};

/**
 * @brief The ConnectableShapeAdapterDecorator class is a simple forward decorator.
 */
class ConnectableShapeAdapterDecorator : public IConnectableShapeAdapter
{
public:
  explicit ConnectableShapeAdapterDecorator(IConnectableShapeAdapter* decoratee);
  ~ConnectableShapeAdapterDecorator() override;

  ConnectableShapeAdapterDecorator(const ConnectableShapeAdapterDecorator&) = delete;
  ConnectableShapeAdapterDecorator& operator=(const ConnectableShapeAdapterDecorator&) = delete;
  ConnectableShapeAdapterDecorator(ConnectableShapeAdapterDecorator&&) = delete;
  ConnectableShapeAdapterDecorator& operator=(ConnectableShapeAdapterDecorator&&) = delete;

  mvvm::SessionItem* GetItem() const override;
  QString GetDisplayName() const override;
  double GetX() const override;
  void SetX(double value) override;
  double GetY() const override;
  void SetY(double value) override;
  std::vector<PortInfo> GetPortInfos() const override;
  NodeOperationStates GetOperationStates() const override;
  void ProcessEvent(const node_event_t& event) const override;
  ConnectableShapeStyle GetStyle() const override;

private:
  IConnectableShapeAdapter* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_CONNECTABLE_SHAPE_ADAPTER_H_
