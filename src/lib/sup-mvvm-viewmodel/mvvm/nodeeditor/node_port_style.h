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

#ifndef MVVM_NODEEDITOR_NODE_PORT_STYLE_H_
#define MVVM_NODEEDITOR_NODE_PORT_STYLE_H_

#include <mvvm/style/color_flavor.h>

#include <QColor>
#include <QRectF>

namespace mvvm
{

/**
 * @brief The NodePortStyle class is an aggregate with all constants related to NodePortShape
 * painting.
 */
struct NodePortStyle
{
  QColor base_color{Qt::gray};
  QColor base_color_modified{Qt::gray};
  double radius{1.0};
  double radius_hovered{1.0};
  QColor pen_color{Qt::gray};
  double pen_width{1.0};
};

/**
 * @brief Creates default node port style for given color flavor.
 */
NodePortStyle CreateDefaultNodePortStyle(ColorFlavor color_flavor = ColorFlavor::kAuto);

/**
 * @brief Returns default node port style.
 */
const NodePortStyle& GetDefaultNodePortStyle();

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_PORT_STYLE_H_
