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

#ifndef MVVM_NODEEDITOR_CONNECTABLE_SHAPE_STYLE_H_
#define MVVM_NODEEDITOR_CONNECTABLE_SHAPE_STYLE_H_

#include <mvvm/style/color_flavor.h>

#include <QColor>
#include <QFont>
#include <QRectF>
#include <QString>

namespace mvvm
{

/**
 * @brief The ConnectableShapeStyle class is an aggregate with all constants related to
 * ConnectableShape painting.
 */
struct ConnectableShapeStyle
{
  QColor base_color;
  QRectF base_rect;
  QColor boundary_color;
  QColor selected_boundary_color;
  bool shadow_enabled{true};
  QColor shadow_color;
  double round_par{5.0};
  double pen_width{1.0};
  double howered_pen_width{1.5};
  QFont label_font;
};

/**
 * @brief Creates default connectable shape style for given color flavor.
 */
ConnectableShapeStyle CreateDefaultConnectableShapeStyle(
    ColorFlavor color_flavor = ColorFlavor::kAuto);

/**
 * @brief Returns default connectable shape style.
 */
const ConnectableShapeStyle& GetDefaultConnectableShapeStyle();

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_CONNECTABLE_SHAPE_STYLE_H_
