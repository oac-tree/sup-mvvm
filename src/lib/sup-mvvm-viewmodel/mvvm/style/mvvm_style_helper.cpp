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

#include "mvvm_style_helper.h"

#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QPalette>

namespace mvvm::style
{

bool IsDarkTheme()
{
  if (QApplication::instance())
  {
    const auto palette = QApplication::palette();
    return palette.color(QPalette::WindowText).lightness()
           > palette.color(QPalette::Window).lightness();
  }

  return false;
}

std::int32_t SystemReferencePointSize()
{
  constexpr std::int32_t kFixedSize = 10;
  return kFixedSize;
}

std::int32_t SystemPointSize()
{
  return QApplication::instance() ? QApplication::font().pointSize() : SystemReferencePointSize();
}

QSize GetUnitSizeFromFont(const QFont &font)
{
  const QFontMetrics font_metric(font);
  auto horizontal_advance = font_metric.horizontalAdvance('M');
  auto font_ascent = font_metric.ascent();
  return {horizontal_advance, font_ascent};
}

QSize FindSizeOfLetterM()
{
  if (QApplication::instance())
  {
    return GetUnitSizeFromFont(QApplication::font());
  }

  // when no QApplication is running return something for purpose of unit tests
  const std::int32_t default_width = 11;
  const std::int32_t default_height = 14;
  return {default_width, default_height};
}

double UnitSize(double scale)
{
  static const QSize kUnitSize = FindSizeOfLetterM();
  return kUnitSize.width() * scale;
}

int32_t UnitRoundSize(double scale)
{
  // simple rounting policy (consider aligning with HighDpiScaleFactorRoundingPolicy)
  return static_cast<std::int32_t>(UnitSize(scale));
}

}  // namespace mvvm::style
