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

#ifndef MVVM_WIDGETS_APPEARANCE_HELPER_H_
#define MVVM_WIDGETS_APPEARANCE_HELPER_H_

//! @file appearance_helper.h
//! Collection of style and color related helper functions.

#include <QString>

class QColor;

namespace mvvm
{

/**
 * @brief Returns a string representing a borderless style for QToolButton.
 *
 * @param button_color A button background collor.
 * @param text_color A button text color.
 */
QString GetFlatButtonStyleString(const QColor& button_color, const QColor& text_color);

/**
 * @brief Returns a string representing a style for QToolButton embedded in a toolbar.
 *
 * It's normally a middle-size button with border when hovered.
 */
QString GetToolBarButtonStyleString();

/**
 * @brief Returns a string representing a borderless style for QToolButton embedded in a status bar.
 *
 * It's normally a tiny button without borders.
 */
QString GetStatusBarButtonStyleString();

}  // namespace mvvm

#endif  // MVVM_WIDGETS_APPEARANCE_HELPER_H_
