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

#include <cstdint>

class QFont;
class QSize;

namespace mvvm::style
{

/**
 * @brief Checks if main application theme is dark.
 */
bool IsDarkTheme();

/**
 * @brief Returns size in points of the "reference" QApplication font.
 *
 * This is a fixed size which never changes. It is used for reference when QApplication is not
 * available.
 */
std::int32_t SystemReferencePointSize();

/**
 * @brief Returns size in points of the current QApplication font.
 */
std::int32_t SystemPointSize();

/**
 * @brief Calculates the size of the letter `M` from the given font.
 */
QSize GetUnitSizeFromFont(const QFont& font);

/**
 * @brief Calculate the size of letter 'M' from system font.
 */
QSize FindSizeOfLetterM();

/**
 * @brief Characteristic size of the UI element scaled by a given factor.
 *
 * This corresponds to the width of the letter 'M' at current application font settings.
 */
double UnitSize(double scale);

/**
 * @brief Characteristic size of the UI element scaled by a given factor.
 *
 * This corresponds to the width of the letter 'M' at current application font settings.
 */
std::int32_t UnitRoundSize(double scale);

}  // namespace mvvm::style

#endif  // MVVM_STYLE_MVVM_STYLE_HELPER_H_
