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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_

//! @file
//! Collection of test strategies for testing purpose.

#include <mvvm/providers/abstract_row_strategy.h>
#include <mvvm/providers/i_children_strategy.h>

namespace mvvm::test
{

/**
 * @brief The EmptyRowTestStrategy class represents broken controller for testing purposes which
 * always returns empty row.
 */
class EmptyRowTestStrategy : public mvvm::AbstractRowStrategy
{
public:
  std::size_t GetSize() const override { return 0U; }

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem*) override;
};

/**
 * @brief The FlatContainerTestStrategy class reports children of ContainerItem in place of the
 * container.
 */
class FlatContainerTestStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;

  mvvm::SessionItem* GetViewParent(mvvm::SessionItem* item) const override;
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_
