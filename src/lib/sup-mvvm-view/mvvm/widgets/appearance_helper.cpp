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

#include "appearance_helper.h"

#include <QApplication>
#include <QColor>
#include <QPalette>

namespace mvvm
{

namespace
{

QString GetBackgroundColorName(const QColor& base_color)
{
  return base_color.name(QColor::HexRgb);
}

QString GetPressedColorName(const QColor& base_color)
{
  return base_color.darker(150).name(QColor::HexRgb);
}

QString GetHoverColorName(const QColor& base_color)
{
  return base_color.lighter(150).name(QColor::HexRgb);
}

QString GetCheckedColorName(const QColor& base_color)
{
  return base_color.lighter(120).name(QColor::HexRgb);
}

QString GetTextColorName(const QColor& text_color)
{
  return text_color.name(QColor::HexRgb);
}

QString GetButtonStyleStringWithHoveredBorders(const QColor& button_color, const QColor& text_color)
{
  static const QString kButtonStyleWithBorder = (R"RAW(
QToolButton { border: none; color: %5; background-color: %1;}
QToolButton:hover { border:1px solid %5; border-radius: 4px; color: %5; background-color: %3; }
QToolButton:pressed { color: %5; background-color: %2; }
QToolButton:checked { color: %5; background-color: %4; }
)RAW");
  return kButtonStyleWithBorder.arg(
      GetBackgroundColorName(button_color), GetPressedColorName(button_color),
      GetHoverColorName(button_color), GetCheckedColorName(button_color),
      GetTextColorName(text_color));
}

QString GetButtonStyleString(const QColor& button_color, const QColor& text_color)
{
  static const QString kButtonStyle = (R"RAW(
QToolButton { border: none; color: %5; background-color: %1;}
QToolButton:hover { color: %5; background-color: %3; }
QToolButton:pressed { color: %5; background-color: %2; }
QToolButton:checked { color: %5; background-color: %4; }
)RAW");
  return kButtonStyle.arg(GetBackgroundColorName(button_color), GetPressedColorName(button_color),
                          GetHoverColorName(button_color), GetCheckedColorName(button_color),
                          GetTextColorName(text_color));
}

}  // namespace

QString GetFlatButtonStyleString(const QColor& button_color, const QColor& text_color)
{
  return GetButtonStyleStringWithHoveredBorders(button_color, text_color);
}

QString GetToolBarButtonStyleString()
{
  const auto palette = QApplication::palette();
  const auto button_color = palette.color(QPalette::Window);
  const auto text_color = palette.color(QPalette::WindowText);
  return GetButtonStyleStringWithHoveredBorders(button_color, text_color);
}

QString GetStatusBarButtonStyleString()
{
  const auto palette = QApplication::palette();
  const auto button_color = palette.color(QPalette::Window);
  const auto text_color = palette.color(QPalette::WindowText);
  return GetButtonStyleString(button_color, text_color);
}

}  // namespace mvvm
