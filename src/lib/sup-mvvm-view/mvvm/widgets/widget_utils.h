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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_WIDGET_UTILS_H_
#define MVVM_WIDGETS_WIDGET_UTILS_H_

//! @file
//! Collection of various widget-related utils.

#include <QString>
#include <QStringList>
#include <string>
#include <vector>

class QColor;
class QSize;
class QMainWindow;
class QLabel;
class QFont;

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
 * @brief Checks if the current host is a Windows machine.
 */
bool IsWindowsHost();

/**
 * @brief Checks if the current host is a Mac.
 */
bool IsMacHost();

/**
 * @brief Checks if the current host is Linux.
 */
bool IsLinuxHost();

/**
 * @brief Returns a string where Linux path to the file is striped using '~/'.
 */
QString WithTildeHomePath(const QString& path);

/**
 * @brief Returns width of the letter 'M' deduced from current font metrics.
 */
int WidthOfLetterM();

/**
 * @brief Returns height of the letter 'M' deduced from current font metrics.
 */
int HeightOfLetterM();

/**
 * @brief Returns size corresponding to the actual size of the letter `M` based on current font
 * metrics.
 */
QSize SizeOfLetterM();

/**
 * @brief Sets the global font for the application.
 */
void SetApplicationFont(const QFont& font);

/**
 * @brief Returns size in points of default system font.
 */
int SystemPointSize();

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
 * @brief Set the label's font size to the system font size scaled by a given factor.
 *
 * @param label The label to change.
 * @param scale Scale font by the given ammount.
 * @param is_bold Allows to turn the label's font into bold.
 */
void ScaleLabelFont(QLabel* label, double scale, bool is_bold = false);

/**
 * @brief Converts vector of strings to QStringList.
 */
QStringList GetStringList(const std::vector<std::string>& vec);

/**
 * @brief Converts string list to std vector.
 */
std::vector<std::string> GetStdStringVector(const QStringList& string_list);

/**
 * @brief Converts vector of strings to byte array.
 */
QByteArray GetByteArray(const QStringList& data);

/**
 * @brief Converts byte array to vector of strings.
 */
QStringList GetStringList(const QByteArray& byteArray);

/**
 * @brief Turn the string containing Unix path into special text intended for clickable labels.
 *
 * Converts
 * "/home/user" -> "/ home / user"
 * and then place links to make text clickable, so the final result will be
 * "/ <a href="/home">home</a> / <a href="/home/user">user</a>"
 */
QString CreatePathPresentation(const QString& text);

/**
 * @brief Returns characteristic UI size scaled by a given factor.
 *
 * Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's own
 * virtual coordinate system. The user is expected to express all widget sizes in these units to be
 * truly independent of any high-DPI issues.
 */
int UnitSize(double scale = 1.0);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_WIDGET_UTILS_H_
