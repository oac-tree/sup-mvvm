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

#include "node_editor_constants.h"

#include <QApplication>
#include <QFont>
#include <QFontMetrics>

namespace mvvm::constants
{

namespace
{

/**
 * @brief Calculate size of typical UI element.
 *
 * We take letter `M` at given QApplication settings.
 */
double CalculateUnitSize()
{
  const double kFallbackValue{11.0};  // typical width of letter 'M' in points

  if (!QApplication::instance())
  {
    return kFallbackValue;
  }

  const QFontMetrics font_metric(QApplication::font());
  const auto horizontal_advance = font_metric.horizontalAdvance('M');

  return static_cast<double>(horizontal_advance);
}

}  // namespace

double NodeUnitSize(double scale)
{
  static const double kUnitSize = CalculateUnitSize();
  return kUnitSize * scale;
}

std::int32_t NodeUnitRoundSize(double scale)
{
  // simple rounting policy (consider aligning with HighDpiScaleFactorRoundingPolicy)
  return static_cast<std::int32_t>(NodeUnitSize(scale));
}

double GetDefaultPortRadius()
{
  return NodeUnitSize(0.4);
}

const QColor &DefaultPortColor()
{
  static const QColor kDefaultPortColor("navajowhite");
  return kDefaultPortColor;
}

const QFont &DefaultPortLabelFont()
{
  static const QFont kLabelFont("Monospace", 8, QFont::Normal);
  return kLabelFont;
}

const QRectF &DefaultConnectableShapeRect()
{
  static const QRectF kDefaultRect{0, 0, NodeUnitSize(8.2), NodeUnitSize(10.0)};
  return kDefaultRect;
}

}  // namespace mvvm::constants
