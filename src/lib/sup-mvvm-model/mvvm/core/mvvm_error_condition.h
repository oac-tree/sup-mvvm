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

#ifndef MVVM_CORE_MVVM_ERROR_CONDITION_H
#define MVVM_CORE_MVVM_ERROR_CONDITION_H

//! @file
//! Contains MvvmErrorCondition enum to inspect/classify error codes.

#include <cstdint>
#include <system_error>

namespace mvvm
{

constexpr auto kModelErrorCondition = "model_error_condition";

/**
 * @brief The MvvmErrorCondition enum introduces error conditions to classify and inspect mvvm
 * errors.
 *
 * Contains only one error category for the moment.
 */
enum class MvvmErrorCondition : std::uint8_t
{
  // no 0
  ModelError = 1,
  MaxCount
};

/**
 * @brief Helper function to create std::error_condition from MvvmErrorCondition enum.
 */
std::error_condition make_error_condition(MvvmErrorCondition condition);

}  // namespace mvvm

namespace std
{

/**
 * @brief Registers MvvmErrorCondition enum as error condition.
 */
template <>
struct is_error_condition_enum<mvvm::MvvmErrorCondition> : true_type
{
};

}  // namespace std

#endif  // MVVM_CORE_MVVM_ERROR_CONDITION_H
