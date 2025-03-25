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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_VIEWMODEL_HELPER_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_VIEWMODEL_HELPER_H_

//! @file test_utils.h
//! Collection of utility functions to simplify viewmodel testing.

#include <mvvm/core/variant.h>

#include <QModelIndex>

namespace mvvm
{
class AllItemsViewModel;
}  // namespace mvvm

namespace mvvm::test
{

/**
 * @brief Test helper that adds given data to a model as PropertyItem.
 *
 * @param data The data to dtore as data role.
 * @param editor_type Optional editor name to define for the given cell.
 * @param view_model Corresponding viewmodel.
 * @return Index of cell which can be used to access given data.
 */
QModelIndex AddDataToModel(const variant_t& data, const std::string& editor_type,
                           AllItemsViewModel& view_model);

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_CONTAINER_HELPER_H_
