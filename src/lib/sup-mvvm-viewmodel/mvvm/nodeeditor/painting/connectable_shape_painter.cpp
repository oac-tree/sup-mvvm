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

#include "connectable_shape_painter.h"

namespace mvvm
{

ConnectableShapePainter::ConnectableShapePainter(ConnectableShapeStyle m_shape_style,
                                                 ConnectableShape* shape)
    : m_shape_style(std::move(m_shape_style)), m_shape(shape)
{
}

void ConnectableShapePainter::Paint(QPainter* painter)
{
  (void)painter;
}

}  // namespace mvvm
