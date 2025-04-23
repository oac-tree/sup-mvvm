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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_CELL_DECORATOR_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_CELL_DECORATOR_H_

#include "mvvm/providers/i_cell_decorator.h"

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockCellDecorator class to test classes relying on ICellDecorator interface.
 */
class MockCellDecorator : public ICellDecorator
{
public:
  MOCK_METHOD(bool, HasCustomDecoration, (const QModelIndex& index), (const, override));
  MOCK_METHOD(void, InitStyleOption, (const QModelIndex& index, QStyleOptionViewItem* option),
              (const, override));
};

/**
 * @brief The CellDecoratorWrapper intended to wrap MockCellDecorator to avoid passing an
 * ownerhship to mock object.
 */
class CellDecoratorWrapper : public ICellDecorator
{
public:
  explicit CellDecoratorWrapper(ICellDecorator* decoratee) : m_decoratee(decoratee) {}

  bool HasCustomDecoration(const QModelIndex& index) const override;

  void InitStyleOption(const QModelIndex& index, QStyleOptionViewItem* option) const override;

private:
  ICellDecorator* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_CELL_DECORATOR_H_
