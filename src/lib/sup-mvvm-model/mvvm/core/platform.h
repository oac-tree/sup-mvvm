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

#ifndef MVVM_CORE_PLATFORM_H_
#define MVVM_CORE_PLATFORM_H_

//! @file
//! Defines collection of functions to get platform and system information.

namespace mvvm
{

/**
 * @brief Checks if we are on Linux operation system.
 */
bool IsLinuxHost();

/**
 * @brief Checks if we are on Windows operation system.
 */
bool IsWindowsHost();

/**
 * @brief Checks if we are on Mac operation system.
 */
bool IsMacHost();

}  // namespace mvvm

#endif  // MVVM_CORE_PLATFORM_H_
