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

#ifndef MVVM_NODEEDITOR_PAINTER_HELPER_H_
#define MVVM_NODEEDITOR_PAINTER_HELPER_H_

//! @file
//! Collection of helper functions for painting.

class QPainterPath;
class QPen;

namespace mvvm
{

/**
 * @brief Scales pen width according to the current high-DPI settings.
 */
double ScalePenWidth(double value, double min_value = 1.0);

/**
 * @brief Scales pen width according to the current high-DPI settings.
 *
 * The value will be rounded halfway, and then min value is applied.
 * 0.1->1.0, 1.0->1.0, 1.25->1.0, 1.5->2.0
 */
double ScaleAndRoundPenWidth(double value, double min_value = 1.0);

/**
 * @brief Returns shape obtained by stroking the given path with the given pen.
 *
 * Function is inspired by the Qt source code (QGraphicsPathItem::qt_graphicsItem_shapeFromPath
 * implementation). The complexity is connected with the fact that QPainterPathStroker does not
 * handle zero-width.
 */
QPainterPath GetShapeFromPath(const QPainterPath& path, const QPen& pen);

/**
 * @brief Returns the shape of node connection using its path.
 */
QPainterPath GetConnectionShape(const QPainterPath& node_connection_path, double width);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTER_HELPER_H_
