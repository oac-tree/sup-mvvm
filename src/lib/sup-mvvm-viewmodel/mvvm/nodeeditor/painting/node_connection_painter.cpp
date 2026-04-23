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

#include "node_connection_painter.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace mvvm
{

NodeConnectionPainter::NodeConnectionPainter(NodeConnectionStyle connection_style)
    : m_connection_style(connection_style)
{
}

void NodeConnectionPainter::Paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                  const QPainterPath& path)
{
  (void)painter;
  painter->setBrush(Qt::NoBrush);

  if (IsSelected(option))
  {
    DrawSelectedLine(painter, path);
  }
  else if (m_is_hovered)
  {
    DrawHoverLine(painter, path);
  }
  else
  {
    DrawBaseLine(painter, path);
  }
}

void NodeConnectionPainter::SetHovered(bool value)
{
  m_is_hovered = value;
}

void NodeConnectionPainter::SetOperationStates(const NodeOperationStates& operation_state)
{
  m_operation_state = operation_state;
}

const QColor& NodeConnectionPainter::GetBaseColor(const NodeOperationStates& operation_states) const
{
  if (operation_states.IsSet(NodeOperationState::kNotFinished))
  {
    return m_connection_style.not_finished_color;
  }

  if (operation_states.IsSet(NodeOperationState::kRunning))
  {
    return m_connection_style.running_color;
  }

  if (operation_states.IsSet(NodeOperationState::kSuccess))
  {
    return m_connection_style.success_color;
  }

  if (operation_states.IsSet(NodeOperationState::kFailure))
  {
    return m_connection_style.failed_color;
  }

  return m_connection_style.base_color;
}

void NodeConnectionPainter::DrawHoverLine(QPainter* painter, const QPainterPath& path) const
{
  // drawing thicker "halo" line
  painter->setPen(QPen(m_connection_style.hover_color, m_connection_style.line_width + 2.0,
                       Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawPath(path);

  // drawing base line on top
  DrawBaseLine(painter, path);
}

void NodeConnectionPainter::DrawSelectedLine(QPainter* painter, const QPainterPath& path) const
{
  painter->setPen(QPen(m_connection_style.base_color, m_connection_style.line_width + 1.0,
                       Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawPath(path);
}

void NodeConnectionPainter::DrawBaseLine(QPainter* painter, const QPainterPath& path) const
{
  const auto color = GetBaseColor(m_operation_state);
  painter->setPen(
      QPen(color, m_connection_style.line_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawPath(path);
}

bool NodeConnectionPainter::IsSelected(const QStyleOptionGraphicsItem* option)
{
  return option->state & (QStyle::State_Selected | QStyle::State_HasFocus);
}

}  // namespace mvvm
