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

#include "node_connection_style.h"

#include <mvvm/nodeeditor/node_editor_constants.h>
#include <mvvm/style/mvvm_style_helper.h>

namespace mvvm
{

namespace
{

/**
 * @brief Creates default style for NodeConnectionShape in light desktop environment.
 */
NodeConnectionStyle CreateDefaultStyleForLightThemes()
{
  NodeConnectionStyle result;
  result.construction_color = QColor("gray");
  result.base_color = QColor("darkcyan");
  result.selected_color = QColor("gray");
  result.selected_boundary_color = QColor("darkorange");
  result.hover_color = QColor("lightcyan");
  result.line_width = 3.0;
  result.construction_line_width = 2.0;
  result.point_diameter = 10.0;
  return result;
}

/**
 * @brief Creates default style for NodeConnectionShape in dark desktop environment.
 */
NodeConnectionStyle CreateDefaultStyleForDarkThemes()
{
  NodeConnectionStyle result = CreateDefaultStyleForLightThemes();
  result.construction_color = QColor("gray");
  result.base_color = QColor("darkcyan");
  result.selected_color = QColor("gray");
  result.selected_boundary_color = QColor("darkorange");
  result.hover_color = QColor("dodgerblue");
  return result;
}

}  // namespace

NodeConnectionStyle CreateDefaultNodeConnectionStyle(ColorFlavor color_flavor)
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

const NodeConnectionStyle &GetDefaultNodeConnectionStyle()
{
  static const auto kStyle = CreateDefaultNodeConnectionStyle(ColorFlavor::kUnspecified);
  return kStyle;
}

}  // namespace mvvm
