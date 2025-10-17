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
  result.base_rect = constants::DefaultConnectableShapeRect();
  result.boundary_color = Qt::darkGray;
  result.selected_boundary_color = QColor("cornflowerblue");
  result.shadow_enabled = false;
  result.shadow_color = Qt::darkGray;
  result.round_par = 5.0;
  result.pen_width = 1.0;
  result.howered_pen_width = 1.5;
  result.label_font = QFont("Noto Sans", constants::NodeUnitRoundSize(0.8), QFont::Normal);

  return result;
}

/**
 * @brief Creates default style for ConnectableShape in dark desktop environment.
 */
ConnectableShapeStyle CreateDefaultStyleForDarkThemes()
{
  ConnectableShapeStyle result = CreateDefaultStyleForLightThemes();
  result.base_color = Qt::darkGray;
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

const ConnectableShapeStyle &GetDefaultConnectableShapeStyle()
{
  static const ConnectableShapeStyle kConnectableShapeStyle =
      CreateDefaultConnectableShapeStyle(ColorFlavor::kUnspecified);
  return kConnectableShapeStyle;
}

}  // namespace mvvm
