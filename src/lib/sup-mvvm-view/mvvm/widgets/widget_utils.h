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

#include <QString>
#include <QStringList>
#include <string>
#include <vector>

class QColor;
class QSize;
class QMainWindow;
class QLabel;
class QFont;

//! Collection of various widget-related utils.

namespace mvvm::utils
{
int RandInt(int low, int high);

//! Returns random color.
QColor RandomColor();

//! Returns the name of random color.
std::string RandomNamedColor();

//! Returns true if it is Windows.
bool IsWindowsHost();

//! Returns true if it is Mac.
bool IsMacHost();

//! Returns true if it is Linux.
bool IsLinuxHost();

//! Returns a string where Linux path to the file is striped using '~/'.
QString WithTildeHomePath(const QString& path);

//! Returns width of the letter 'M' deduced from current font metrics.
int WidthOfLetterM();

//! Returns height of the letter 'M' deduced from current font metrics.
int HeightOfLetterM();

//! Returns size corresponding to actual size of letter `M` basing on current font metrics.
QSize SizeOfLetterM();

//! Sets global font for the application.
void SetApplicationFont(const QFont& font);

//! Returns size in points of default system font.
int SystemPointSize();

//! Finds main window.
QMainWindow* FindMainWindow();

//! Returns text wrapped into 'href' tag to provide clickable links in QLabel.
//! Example: <a href="tag">text</a>, if 'tag' is empty, 'text' will be used instead.
QString ClickableText(const QString& text, const QString& tag = {});

//! Set label's font size to system font size scaled by given factor.
void ScaleLabelFont(QLabel* label, double scale, bool is_bold = false);

//! Converts vector of strings to QStringList.
QStringList GetStringList(const std::vector<std::string>& vec);

//! Converts vector of strings to QStringList.
std::vector<std::string> GetStdStringVector(const QStringList& string_list);

//! Converts vector of strings to byte array.
QByteArray GetByteArray(const QStringList& data);

//! Converts byte array to vector of strings.
QStringList GetStringList(const QByteArray& byteArray);

//! Create path presentation.
//! Converts "/home/user" -> "/ home / user" and then place links to make text clickable, so
//! the final result will be R"(/ <a href="/home">home</a> / <a href="/home/user">user</a>)"
QString CreatePathPresentation(const QString& text);

//! Returns characteristic UI size scaled by a given factor.
//! Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's
//! own virtual coordinate system. The user is expected to express all widget sizes in these units.
int UnitSize(double scale = 1.0);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_WIDGET_UTILS_H_
