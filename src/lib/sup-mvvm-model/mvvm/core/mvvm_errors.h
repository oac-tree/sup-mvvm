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

#ifndef MVVM_CORE_MVVM_ERRORS_H
#define MVVM_CORE_MVVM_ERRORS_H

//! @file
//! Contains ModelError enum with all model-related errors, and the machinery to register it as
//! std::error_code.

#include <cstdint>
#include <system_error>

namespace mvvm
{

constexpr auto kModelErrorCategory = "model_error_category";
constexpr auto kUnknownErrorMessage = "unknown error";

/**
 * @brief The ModelError enum defines sup-mvvm error codes.
 */
enum class ModelError : std::uint8_t
{
  Success = 0,
  NullItem,
  ItemAndParentAreTheSame,
  ItemHasNoParent,
  ItemHasParent,
  InvalidParentItem,
  ItemIsAncestorOfNewParent,
  FailedToInsertItem,
  ItemBelongToDifferentModel,
  FailedToTakeItem,
  FailedToMoveItem,
  InvalidChildItem,
  MaxCount
};

/**
 * @brief Helper function to create std::error_code from our enums.
 */
std::error_code make_error_code(ModelError model_error);

}  // namespace mvvm

namespace std
{

/**
 * @brief Registers mvvm::ModelError enum with std::error_code.
 */
template <>
struct is_error_code_enum<mvvm::ModelError> : true_type
{
};

}  // namespace std

#endif  // MVVM_CORE_MVVM_ERRORS_H
