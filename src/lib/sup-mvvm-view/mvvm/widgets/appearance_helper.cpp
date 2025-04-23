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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "appearance_helper.h"

#include <QColor>

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

}  // namespace

QString GetFlatButtonStyleString(const QColor& background_color)
{
  static const QString result =
      "QToolButton { border: none; color: white; background-color: %1;}"
      "QToolButton:pressed { color: black; background-color: %2; }"
      "QToolButton:hover { color: white; background-color: %3; }"
      "QToolButton:checked { color: white; background-color: %4; }";
  return result.arg(GetBackgroundColorName(background_color), GetPressedColorName(background_color),
                    GetHoverColorName(background_color), GetCheckedColorName(background_color));
}

}  // namespace mvvm
