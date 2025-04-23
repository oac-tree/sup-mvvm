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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_

//! @file
//! Collection of test strategies for testing purpose.

#include <mvvm/viewmodel/abstract_row_strategy.h>

namespace mvvm::test
{

/**
 * @brief The EmptyRowTestStrategy class represents broken controller for testing purposes which
 * always returns empty row.
 */
class EmptyRowTestStrategy : public mvvm::AbstractRowStrategy
{
public:

  int GetSize() const override { return 0;}

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem*) override;
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_STRATEGIES_H_
