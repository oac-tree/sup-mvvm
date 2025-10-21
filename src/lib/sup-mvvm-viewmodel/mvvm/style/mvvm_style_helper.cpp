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
#include <QLabel>
#include <QPalette>

namespace mvvm::style
{

namespace
{
//! Stores the value of "UnitSize" constants in a static variable when true. This allows us to
//! provide faster access to the often-used variable. The downside is that after the application
//! restarts with another font size setting (without exiting main()), the size of UI elements might
//! remain without additional scaling.
bool kStoreUniSizeInStaticConst = false;
}  // namespace

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

std::int32_t SystemPointSize(double scale)
{
  const std::int32_t current_psize =
      QApplication::instance() ? QApplication::font().pointSize() : SystemReferencePointSize();
  return static_cast<std::int32_t>(static_cast<double>(current_psize) * scale);
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

double UnitSizeF(double scale)
{
  if (kStoreUniSizeInStaticConst)
  {
    // much faster access
    static const QSize kUnitSizeStatic = FindSizeOfLetterM();
    return kUnitSizeStatic.width() * scale;
  }
  else
  {
    return FindSizeOfLetterM().width() * scale;
  }
}

int32_t UnitSize(double scale)
{
  // simple rounding policy (consider aligning with HighDpiScaleFactorRoundingPolicy)
  return static_cast<std::int32_t>(UnitSizeF(scale));
}

double ScaleToSystem(double device_independent_pixels)
{
  return device_independent_pixels * static_cast<double>(SystemPointSize())
         / static_cast<double>(SystemReferencePointSize());
}

void ScaleLabelFont(QLabel &label, double scale, bool is_bold)
{
  QFont font = label.font();
  font.setPointSize(style::SystemPointSize(scale));
  if (is_bold)
  {
    font.setBold(true);
  }
  label.setFont(font);
}

}  // namespace mvvm::style
