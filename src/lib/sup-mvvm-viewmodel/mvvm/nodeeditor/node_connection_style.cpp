/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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
  result.base_color = QColor("gray");
  result.hover_color = QColor("slateblue");

  // better to match colors with ConnectableShapeStyle
  result.not_finished_color = QColor("darkorange");
  result.running_color = QColor("darkorange");
  result.success_color = QColor("darkgreen");
  result.failed_color = QColor("darkred");

  result.line_width = style::ScaleToSystem(2.0);
  return result;
}

/**
 * @brief Creates default style for NodeConnectionShape in dark desktop environment.
 */
NodeConnectionStyle CreateDefaultStyleForDarkThemes()
{
  const auto light_style = CreateDefaultStyleForLightThemes();

  NodeConnectionStyle result = CreateDefaultStyleForLightThemes();
  result.base_color = Qt::lightGray;
  result.hover_color = QColor("floralwhite");
  result.not_finished_color = QColor("orange");
  result.running_color = light_style.running_color.lighter();
  result.success_color = light_style.success_color.lighter(200);
  result.failed_color = light_style.failed_color.lighter();

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

const NodeConnectionStyle& GetDefaultNodeConnectionStyle()
{
  static const auto kStyle = CreateDefaultNodeConnectionStyle(ColorFlavor::kAuto);
  return kStyle;
}

}  // namespace mvvm
