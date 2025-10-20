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
class QLabel;

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
 * @brief Returns size in points of the current QApplication font scaled by the given ammount.
 */
std::int32_t SystemPointSize(double scale = 1.0);

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
 * This corresponds to the width of the letter 'M' at current application font settings, multipled
 * by a scla e factor.
 */
double UnitSize(double scale = 1.0);

/**
 * @brief Characteristic size of the UI element scaled by a given factor.
 *
 * This corresponds to the width of the letter 'M' at current application font settings.
 */
std::int32_t UnitRoundSize(double scale = 1.0);

/**
 * @brief Scales the given value according to the system font size.
 *
 * This method is used to scale Qt UI elements' sizes so that they look consistent on systems with
 * different font size settings.
 *
 * Qt widget sizes are given in device-independent pixels. They will be already scaled according to
 * the system DPI settings by the Qt framework. However, different systems may have different
 * default font sizes. This method allows for scaling sizes further according to the system font
 * size.
 *
 * @param device_independent_pixels Size of widgets in device-independent pixels.
 * @return Size scaled to the current system font.
 */
double ScaleToSystemFont(double device_independent_pixels);

/**
 * @brief Set the label's font size to the system font size scaled by a given factor.
 *
 * @param label The label to change.
 * @param scale Scale font by the given ammount.
 * @param is_bold Allows to turn the label's font into bold.
 */
void ScaleLabelFont(QLabel& label, double scale, bool is_bold = false);

}  // namespace mvvm::style

#endif  // MVVM_STYLE_MVVM_STYLE_HELPER_H_
