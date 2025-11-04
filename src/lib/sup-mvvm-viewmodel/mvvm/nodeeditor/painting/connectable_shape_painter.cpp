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

#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace mvvm
{

namespace
{

/**
 * @brief Returns rectangle to display ConnectableShape label.
 *
 * The label will be occupying top of the bounding box rectangle of a shape.
 */
QRectF GetLabelRectangle(const QRectF& rect)
{
  // take a rectangle which is a half in height of the view's rectangle
  const QRectF result(rect.x(), rect.y(), rect.width(), rect.height() / 2);

  // slightly shrink it
  auto side_gap = style::UnitSizeF(0.25);
  auto top_gap = style::UnitSizeF(0.75);
  return result.marginsRemoved(
      QMarginsF(side_gap, top_gap, side_gap, 0.0));  // left, top, right, bottom
}

}  // namespace

ConnectableShapePainter::ConnectableShapePainter(QGraphicsItem& shape,
                                                 IConnectableShapeAdapter& shape_adapter)
    : m_shape_adapter(shape_adapter), m_shape_style(shape_adapter.GetStyle())
{
  InstallGraphicsEffects(shape);
}

void ConnectableShapePainter::Paint(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  PaintRectangle(painter, option);
  PaintLabel(painter, option);

  painter->restore();
}

void ConnectableShapePainter::SetHovered(bool value)
{
  m_is_hovered = value;
}

const QRectF& ConnectableShapePainter::GetShapeRect() const
{
  return m_shape_style.base_rect;
}

const QColor& ConnectableShapePainter::GetBoundaryColor(
    const NodeOperationStates& operation_states) const
{
  if (operation_states.IsSet(NodeOperationState::kNotFinished))
  {
    return m_shape_style.not_finished_boundary_color;
  }

  if (operation_states.IsSet(NodeOperationState::kRunning))
  {
    return m_shape_style.running_boundary_color;
  }

  if (operation_states.IsSet(NodeOperationState::kSuccess))
  {
    return m_shape_style.success_boundary_color;
  }

  if (operation_states.IsSet(NodeOperationState::kFailure))
  {
    return m_shape_style.failed_boundary_color;
  }

  return m_shape_style.boundary_color;
}

void ConnectableShapePainter::InstallGraphicsEffects(QGraphicsItem& shape) const
{
  if (m_shape_style.shadow_enabled)
  {
    auto effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4, 4);
    effect->setBlurRadius(20);
    effect->setColor(m_shape_style.shadow_color);
    shape.setGraphicsEffect(effect);
  }
}

bool ConnectableShapePainter::IsSelected(const QStyleOptionGraphicsItem* option)
{
  return option->state & (QStyle::State_Selected | QStyle::State_HasFocus);
}

void ConnectableShapePainter::PaintRectangle(QPainter* painter,
                                             const QStyleOptionGraphicsItem* option) const
{
  QPen pen;
  pen.setWidthF(m_shape_style.pen_width);
  if (IsSelected(option))
  {
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(m_shape_style.pen_width + 1.0);
  }

  auto color = GetBoundaryColor(m_shape_adapter.GetOperationStates());
  if (m_is_hovered)
  {
    color = color.darker();
  }

  pen.setColor(color);
  painter->setPen(pen);

  painter->setBrush(ConnectableShapeGradient(m_shape_style.base_color, GetShapeRect()));
  painter->drawRoundedRect(GetShapeRect(), m_shape_style.round_par, m_shape_style.round_par);
}

void ConnectableShapePainter::PaintLabel(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
  (void)option;
  const QRectF label_rect = GetLabelRectangle(GetShapeRect());
  painter->setFont(m_shape_style.label_font);
  painter->setPen(Qt::black);
  QTextOption text_option(Qt::AlignHCenter | Qt::AlignTop);
  text_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  painter->drawText(label_rect, m_shape_adapter.GetDisplayName(), text_option);
}

}  // namespace mvvm
