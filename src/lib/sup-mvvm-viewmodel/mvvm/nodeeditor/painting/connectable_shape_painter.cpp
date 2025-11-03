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

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QGraphicsDropShadowEffect>
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

ConnectableShapePainter::ConnectableShapePainter(ConnectableShapeStyle m_shape_style,
                                                 ConnectableShape* shape)
    : m_shape_style(std::move(m_shape_style)), m_shape(shape)
{
  InstallGraphicsEffects();
}

void ConnectableShapePainter::Paint(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
  painter->save();

  painter->setRenderHint(QPainter::Antialiasing);

  QPen pen;
  pen.setWidthF(m_shape_style.pen_width);
  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    pen.setColor(m_shape_style.selected_boundary_color);
    pen.setStyle(Qt::DashLine);
  }
  else
  {
    pen.setColor(m_shape_style.boundary_color);
  }
  painter->setPen(pen);

  painter->setBrush(ConnectableShapeGradient(m_shape_style.base_color, GetShapeRect()));
  painter->drawRoundedRect(GetShapeRect(), m_shape_style.round_par, m_shape_style.round_par);

  // plotting label
  const QFont label_font = m_shape_style.label_font;
  const QRectF label_rect = GetLabelRectangle(GetShapeRect());
  painter->setFont(label_font);
  painter->setPen(Qt::black);
  QTextOption text_option(Qt::AlignHCenter | Qt::AlignTop);
  text_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  painter->drawText(label_rect, GetConnectableShapeAdapter()->GetDisplayName(), text_option);

  painter->restore();
}

QRectF ConnectableShapePainter::GetShapeRect() const
{
  return m_shape_style.base_rect;
}

void ConnectableShapePainter::InstallGraphicsEffects()
{
  if (m_shape_style.shadow_enabled)
  {
    auto effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4, 4);
    effect->setBlurRadius(20);
    effect->setColor(m_shape_style.shadow_color);
    m_shape->setGraphicsEffect(effect);
  }
}

IConnectableShapeAdapter* ConnectableShapePainter::GetConnectableShapeAdapter()
{
  return m_shape->GetConnectableShapeAdapter();
}

}  // namespace mvvm
