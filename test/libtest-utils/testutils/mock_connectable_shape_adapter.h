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

namespace mvvm::test
{

/**
 * @brief The MockConnectableShapeAdapter alows to validate calls to IConnectableShapeAdapter.
 */
class MockConnectableShapeAdapter : public IConnectableShapeAdapter
{
public:
  MOCK_METHOD(QString, GetDisplayName, (), (const, override));
  MOCK_METHOD(QColor, GetBaseColor, (), (const, override));
  MOCK_METHOD(double, GetX, (), (const, override));
  MOCK_METHOD(void, SetX, (double), (override));
  MOCK_METHOD(double, GetY, (), (const, override));
  MOCK_METHOD(void, SetY, (double), (override));
};

class ConnectableShapeDecorator : public IConnectableShapeAdapter
{
public:
  explicit ConnectableShapeDecorator(IConnectableShapeAdapter* decoratee);
  ~ConnectableShapeDecorator() override;

  QString GetDisplayName() const override;
  QColor GetBaseColor() const override;
  double GetX() const override;

  void SetX(double value) override;
  double GetY() const override;
  void SetY(double value) override;

private:
  IConnectableShapeAdapter* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_CONNECTABLE_SHAPE_ADAPTER_H_
