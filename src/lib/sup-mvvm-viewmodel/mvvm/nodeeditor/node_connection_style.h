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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_STYLE_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_STYLE_H_

#include <mvvm/style/color_flavor.h>

#include <QColor>

namespace mvvm
{

/**
 * @brief The NodeConnectionStyle class is an aggregate with all constants related to
 * NodeConnectionShape painting.
 */
struct NodeConnectionStyle
{
  QColor construction_color{Qt::gray};
  QColor base_color{Qt::gray};
  QColor hover_color{Qt::gray};
  QColor not_finished_color{Qt::gray};
  QColor running_color{Qt::gray};
  QColor success_color{Qt::gray};
  QColor failed_color{Qt::gray};
  double line_width{2.0};
};

/**
 * @brief Creates default node port style for given color flavor.
 */
NodeConnectionStyle CreateDefaultNodeConnectionStyle(
    ColorFlavor color_flavor = ColorFlavor::kAuto);

/**
 * @brief Returns default node port style.
 */
const NodeConnectionStyle& GetDefaultNodeConnectionStyle();

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_STYLE_H_
