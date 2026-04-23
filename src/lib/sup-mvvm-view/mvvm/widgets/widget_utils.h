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

#ifndef MVVM_WIDGETS_WIDGET_UTILS_H_
#define MVVM_WIDGETS_WIDGET_UTILS_H_

//! @file
//! Collection of various widget-related utils.

#include <QString>

class QColor;
class QMainWindow;

namespace mvvm::utils
{

/**
 * @brief Returns random color.
 */
QColor RandomColor();

/**
 * @brief Returns the name of the random color.
 */
std::string RandomNamedColor();

/**
 * @brief Returns a string where Linux path to the file is striped using '~/'.
 */
QString WithTildeHomePath(const QString& path);

/**
 * @brief Finds main window.
 */
QMainWindow* FindMainWindow();

/**
 * @brief Returns text wrapped into html link tags.
 *
 * Used solely to provide clickable QLabel.
 *
 * Examples:
 * ClickableText("text") -> <a href="text">text</a>
 * ClickableText("text", "site.com") -> <a href="site.com">text</a>
 */
QString ClickableText(const QString& text, const QString& link = {});

/**
 * @brief Turn the string containing Unix path into special text intended for clickable labels.
 *
 * Converts
 * "/home/user" -> "/ home / user"
 * and then place links to make text clickable, so the final result will be
 * "/ <a href="/home">home</a> / <a href="/home/user">user</a>"
 */
QString CreatePathPresentation(const QString& text);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_WIDGET_UTILS_H_
