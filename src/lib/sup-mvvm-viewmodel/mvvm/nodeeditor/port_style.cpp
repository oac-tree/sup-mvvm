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

#include "port_style.h"

#include <mvvm/nodeeditor/node_editor_constants.h>

namespace mvvm
{

PortStyle CreateDefaultPortStyle()
{
  PortStyle result;

  result.base_color = QColor("navajowhite");
  result.base_color_modified = QColor("orange");
  result.radius = constants::GetDefaultPortRadius();
  result.radius_hovered = constants::GetDefaultPortRadius();
  result.pen_color = QColor("navajowhite").darker(180);
  result.pen_width = 1.0;

  return result;
}

}  // namespace mvvm
