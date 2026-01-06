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

#ifndef MVVM_UTILS_BIN_UTILS_H_
#define MVVM_UTILS_BIN_UTILS_H_

//! @file
//! Utility functions to analyze files.

#include <string>

namespace mvvm::utils
{

/**
 * @brief Checks if a given path is looking to a binary file.
 */
bool IsBinaryFile(const std::string& file_name);

/**
 * @brief Checks if a given path is looking to a text file.
 */
bool IsTextFile(const std::string& file_name);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_BIN_UTILS_H_
