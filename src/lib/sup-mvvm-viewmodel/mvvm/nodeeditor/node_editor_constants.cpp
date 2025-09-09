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

  return horizontal_advance;
}

/**
 * @brief Returns default gradient for connectable shapes.
 */
QGradient CalculateGradient(const QColor &base_color, const QRectF &rect)
{
  QLinearGradient result(rect.x() + rect.width() / 2, rect.y(), rect.x() + rect.width() / 2,
                         rect.y() + rect.height());
  result.setColorAt(0, base_color);
  result.setColorAt(0.5, base_color.lighter(150));
  result.setColorAt(1, base_color);
  return result;
}

}  // namespace

double NodeUnitSize(double scale)
{
  static const double kUnitSize = CalculateUnitSize();
  return kUnitSize * scale;
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

const QGradient &ConnectableShapeGradient(const QColor &color, const QRectF &rect)
{
  static const QGradient kGradient = CalculateGradient(color, rect);
  return kGradient;
}

}  // namespace mvvm::constants
