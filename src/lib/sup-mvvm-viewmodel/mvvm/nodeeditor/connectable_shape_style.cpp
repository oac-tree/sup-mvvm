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

#include "connectable_shape_style.h"

#include "node_editor_constants.h"
#include "painter_helper.h"

#include <mvvm/style/mvvm_style_helper.h>

namespace mvvm
{

namespace
{

/**
 * @brief Creates default style for ConnectableShape in light desktop environment.
 */
ConnectableShapeStyle CreateDefaultStyleForLightThemes()
{
  ConnectableShapeStyle result;

  result.base_color = Qt::lightGray;
  result.base_rect = constants::GetDefaultConnectableShapeRect();
  result.boundary_color = Qt::darkGray;
  result.selected_boundary_color = QColor("cornflowerblue");
  result.not_finished_boundary_color = QColor("darkorange");
  result.running_boundary_color = QColor("darkorange");
  result.success_boundary_color = QColor("darkgreen");
  result.failed_boundary_color = QColor("darkred");
  result.shadow_enabled = true;
  result.shadow_color = Qt::lightGray;
  result.round_par = style::UnitSize(0.5);
  result.pen_width = ScalePenWidth(1.5);
  result.howered_pen_width = ScalePenWidth(1.5);
  result.label_font = constants::GetDefaultConnectableShapeFont();

  return result;
}

/**
 * @brief Creates default style for ConnectableShape in dark desktop environment.
 */
ConnectableShapeStyle CreateDefaultStyleForDarkThemes()
{
  ConnectableShapeStyle result = CreateDefaultStyleForLightThemes();
  result.base_color = Qt::darkGray;
  result.shadow_color = Qt::black;
  return result;
}

}  // namespace

ConnectableShapeStyle CreateDefaultConnectableShapeStyle(ColorFlavor color_flavor)
{
  if (color_flavor == ColorFlavor::kForDarkThemes)
  {
    return CreateDefaultStyleForDarkThemes();
  }

  if (color_flavor == ColorFlavor::kForLightThemes)
  {
    return CreateDefaultStyleForLightThemes();
  }

  return style::IsDarkTheme() ? CreateDefaultStyleForDarkThemes()
                              : CreateDefaultStyleForLightThemes();
}

const ConnectableShapeStyle& GetDefaultConnectableShapeStyle()
{
  static const ConnectableShapeStyle kConnectableShapeStyle =
      CreateDefaultConnectableShapeStyle(ColorFlavor::kAuto);
  return kConnectableShapeStyle;
}

}  // namespace mvvm
