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

#ifndef MVVM_CORE_BASIC_SCALAR_TYPES_H_
#define MVVM_CORE_BASIC_SCALAR_TYPES_H_

/**
 * @file
 * Defines basic scalar types supported by the framework. Definitions are taken as it is from
 * sup-dto framework.
 */

#include <cstdint>

namespace mvvm
{

using boolean = bool;
using char8 = char;
using int8 = std::int8_t;
using uint8 = std::uint8_t;
using int16 = std::int16_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using uint32 = std::uint32_t;
using int64 = std::int64_t;
using uint64 = std::uint64_t;
using float32 = float;
using float64 = double;

/**
 * @brief Returns the numeric value of a char8 as a portable signed integer.
 *
 * char8 is an alias for plain char, whose signedness is implementation-defined. This helper commits
 * to a signed interpretation via signed char, so its numeric value can be used portably (MISRA C++
 * 2023 Rule 7.0.3 forbids using the numeric value of a plain character directly).
 */
constexpr std::int32_t GetCharNumericValue(char8 value) noexcept
{
  return static_cast<std::int32_t>(static_cast<signed char>(value));
}

}  // namespace mvvm

#endif  // MVVM_CORE_BASIC_SCALAR_TYPES_H_
