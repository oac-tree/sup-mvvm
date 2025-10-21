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
 * @brief Returns unit size of the "reference" QApplication font.
 *
 * It represents a width of the letter 'M' in device independent pixels.
 *
 * This is a fixed size which never changes. It is used for reference when QApplication is not
 * available.
 */
std::int32_t SystemReferenceUnitSize();

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
 * Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's own
 * virtual coordinate system. The user might want to express all widget sizes in these units to be
 * truly independent of any high-DPI issues.
 *
 * @see ScaleToSystemFont(double device_independent_pixels)
 */
double UnitSizeF(double scale = 1.0);

/**
 * @brief Characteristic size of the UI element scaled by a given factor (int version).
 *
 * Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's own
 * virtual coordinate system. The user might want to express all widget sizes in these units to be
 * truly independent of any high-DPI issues.
 *
 * @see ScaleToSystemFont(double device_independent_pixels)
 */
std::int32_t UnitSize(double scale = 1.0);

/**
 * @brief Scales the given value according to the system High DPI settings.
 *
 * This method is used to scale Qt UI elements' sizes so that they look consistent on systems with
 * different system font and HighDPI settings.
 *
 * @param device_independent_pixels Size of UI elements in device-independent pixels.
 * @return Size scaled to the current system setting.
 *
 * @details Qt widget sizes are given in device-independent pixels. During the painting they will be
 * already scaled according to the system DPI settings by the Qt framework. See
 * https://doc.qt.io/qt-6/highdpi.html for more details.
 *
 * This function adds additional scaling based on the real physical size of the current system font
 * size. This addreses two issues:
 * 1) Different systems may have different optimal physical sizes for system fonts. In this case we
 * want to scale all other UI elements proportionally to the system font size.
 * 2) On some systems (especially running in the containers) the device pixel ratio reported by the
 * system may be wrong. In this case we want to additionally scale the UI elements to achieve
 * correct physical size.
 *
 * @details The logic of using this function is the following. When you have a size of some UI
 * element set to 64 pixels (i.e. widget.setFixedWidth(64)), and the element looks great on your
 * machine, the safest would be to introduce additional scaling by
 * widget.setFixedWidth(ScaleToSystem(64)). This will ensure that on other systems (even with a
 * screwed device-pixel-ratio) the element will still look great.
 *
 * @see UnitSize(double scale)
 */
double ScaleToSystem(double device_independent_pixels);

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
