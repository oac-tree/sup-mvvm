/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#ifndef MVVM_STYLE_MVVM_STYLE_HELPER_H_
#define MVVM_STYLE_MVVM_STYLE_HELPER_H_

//!< @file
//! Collection of helper functions related to QApplication overall style.

namespace mvvm::style
{

/**
 * @brief Checks if main application theme is dark.
 */
bool IsDarkTheme();

}  // namespace mvvm

#endif  // MVVM_STYLE_MVVM_STYLE_HELPER_H_
