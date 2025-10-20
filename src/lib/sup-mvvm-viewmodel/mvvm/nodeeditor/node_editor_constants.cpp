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

namespace mvvm::constants
{

double GetDefaultPortRadius()
{
  return style::ScaleToSystem(5);
}

const QFont &DefaultConnectableShapeFont()
{
  static const QFont kLabelFont("Noto Sans", style::SystemPointSize(1.0), QFont::Normal);
  return kLabelFont;
}

const QFont &DefaultPortLabelFont()
{
  static const QFont kLabelFont("Monospace", style::SystemPointSize(0.8), QFont::Normal);
  return kLabelFont;
}

const QRectF &DefaultConnectableShapeRect()
{
  static const QRectF kDefaultRect{0, 0, style::ScaleToSystem(100), style::ScaleToSystem(130)};
  return kDefaultRect;
}

}  // namespace mvvm::constants
