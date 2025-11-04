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

#include "node_connection_painter.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>

namespace mvvm
{

NodeConnectionPainter::NodeConnectionPainter()
    : m_connection_style(CreateDefaultNodeConnectionStyle())
{
}

void NodeConnectionPainter::Paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                  const QPainterPath& path)
{
  (void)painter;
  (void)option;

  painter->setPen(QPen(m_connection_style.base_color, m_connection_style.line_width, Qt::SolidLine,
                       Qt::RoundCap, Qt::RoundJoin));
  painter->setBrush(Qt::NoBrush);

  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    painter->setPen(QPen(m_connection_style.selected_color, m_connection_style.line_width,
                         Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
  }

  painter->drawPath(path);
}

void NodeConnectionPainter::SetHovered(bool value)
{
  m_is_hovered = value;
}

}  // namespace mvvm
