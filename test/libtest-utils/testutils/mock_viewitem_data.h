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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_VIEWITEM_DATA_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_VIEWITEM_DATA_H_

#include <mvvm/providers/i_viewitem_data.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockViewItemData class to test IViewItemData interface.
 */
class MockViewItemData : public IViewItemData
{
public:
  MOCK_METHOD(QVariant, Data, (role_t), (const, override));
  MOCK_METHOD(bool, SetData, (const QVariant&, role_t qt_role), (override));
  MOCK_METHOD(bool, IsEnabled, (), (const, override));
  MOCK_METHOD(bool, IsEditable, (), (const, override));
  MOCK_METHOD(bool, IsCheckable, (), (const, override));
};

/**
 * @brief The ViewItemDataDecorator class is intended to wrap MockViewItemData to avoid passing an
 * ownerhship to mock object.
 */
class ViewItemDataDecorator : public IViewItemData
{
public:
  explicit ViewItemDataDecorator(IViewItemData* decoratee) : m_decoratee(decoratee) {}

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& value, role_t qt_role) override;

  bool IsEnabled() const override;

  bool IsEditable() const override;

  bool IsCheckable() const override;

private:
  IViewItemData* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_VIEWITEM_DATA_H_
