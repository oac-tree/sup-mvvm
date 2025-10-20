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

#include <mvvm/style/mvvm_style_helper.h>

#include <QApplication>
#include <QFont>
#include <QFontMetrics>

namespace mvvm::constants
{

double GetDefaultPortRadius()
{
  return style::UnitSizeF(0.4);
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
  static const QRectF kDefaultRect{0, 0, style::UnitSizeF(8.2), style::UnitSizeF(10.0)};
  return kDefaultRect;
}

}  // namespace mvvm::constants
