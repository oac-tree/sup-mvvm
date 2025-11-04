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
  // Fixed reference size representing point size of the default font on a typical system.
  constexpr std::int32_t kFixedSize = 10;
  return kFixedSize;
}

int32_t SystemReferenceUnitSize()
{
  // Fixed reference size representing width of letter 'M' in device independent pixels on a typical
  // system.
  constexpr std::int32_t kFixedSize = 12;
  return kFixedSize;
}

std::int32_t SystemPointSize(double scale)
{
  const std::int32_t current_psize =
      QApplication::instance() ? QApplication::font().pointSize() : SystemReferencePointSize();
  return static_cast<std::int32_t>(static_cast<double>(current_psize) * scale);
}

QSize GetUnitSizeFromFont(const QFont& font)
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
  const std::int32_t default_width = SystemReferenceUnitSize();
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
  return FindSizeOfLetterM().width() * scale;
}

int32_t UnitSize(double scale)
{
  // simple rounding policy (consider aligning with HighDpiScaleFactorRoundingPolicy)
  return static_cast<std::int32_t>(UnitSizeF(scale));
}

double ScaleToSystem(double device_independent_pixels)
{
  constexpr bool kScaleAccordingToPointSize = false;

  if (kScaleAccordingToPointSize)
  {
    // Scaling according to the default system font point size.
    return device_independent_pixels * static_cast<double>(SystemPointSize())
           / static_cast<double>(SystemReferencePointSize());
  }

  // On CODAC systems and CODAC containers, we prefer scaling according to 'M' size rather than
  // point size. This is related to the fact that, depending on the arrangement of stars in the sky,
  // on CODAC the same point size might correspond to different physical sizes of characters.

  return device_independent_pixels * static_cast<double>(UnitSize())
         / static_cast<double>(SystemReferenceUnitSize());
}

void ScaleLabelFont(QLabel& label, double scale, bool is_bold)
{
  QFont font = label.font();
  font.setPointSize(style::SystemPointSize(scale));
  if (is_bold)
  {
    font.setBold(true);
  }
  label.setFont(font);
}

QColor MakeContrastColor(const QColor& color, int factor)
{
  return IsDarkTheme() ? color.lighter(factor) : color.darker(factor);
}

}  // namespace mvvm::style
