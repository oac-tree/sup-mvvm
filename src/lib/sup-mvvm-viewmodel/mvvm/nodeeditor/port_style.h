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

#ifndef MVVM_NODEEDITOR_PORT_STYLE_H_
#define MVVM_NODEEDITOR_PORT_STYLE_H_

#include <QColor>
#include <QRectF>
#include <QString>

namespace mvvm
{

/**
 * @brief The PortStyle class is an aggregate with all constants related to NodePortShape painting.
 */
struct PortStyle
{
  QColor base_color{Qt::gray};
  QColor base_color_modified{Qt::gray};
  double radius{1.0};
  double radius_hovered{1.0};
  QColor pen_color{Qt::gray};
  double pen_width{1.0};
};

/**
 * @brief Returns default port style.
 */
PortStyle CreateDefaultPortStyle();

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_STYLE_H_
