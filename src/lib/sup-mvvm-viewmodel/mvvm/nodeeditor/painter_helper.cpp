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

#include "painter_helper.h"

#include <mvvm/style/mvvm_style_helper.h>

#include <QPainterPath>
#include <QPen>
#include <algorithm>
#include <cmath>

namespace mvvm
{

double ScalePenWidth(double value, double min_value)
{
  return std::max(min_value, style::ScaleToSystem(value));
}

double ScaleAndRoundPenWidth(double value, double min_value)
{
  return std::max(min_value, std::round(style::ScaleToSystem(value)));
}

QPainterPath GetShapeFromPath(const QPainterPath& path, const QPen& pen)
{
  // to bypass QPainterPathStroker behavior, which use 1.0 instead of 0.0
  const qreal pen_width_zero{1e-08};

  if (path == QPainterPath() || pen == Qt::NoPen)
  {
    return path;
  }
  QPainterPathStroker stroker;
  stroker.setCapStyle(pen.capStyle());
  stroker.setWidth(std::max(pen_width_zero, pen.widthF()));
  stroker.setJoinStyle(pen.joinStyle());
  stroker.setMiterLimit(pen.miterLimit());

  return stroker.createStroke(path);
}

QPainterPath GetConnectionShape(const QPainterPath& node_connection_path, double width)
{
  const QPen pen(Qt::black, width, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
  return GetShapeFromPath(node_connection_path, pen);
}

}  // namespace mvvm
