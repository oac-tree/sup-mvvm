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

#include "port_appearance.h"

#include <QFontMetrics>

namespace mvvm
{

namespace
{
double kDefaultPortRadius = 10.0;
}

PortAppearance::PortAppearance(PortLocation port_location, const QString &port_label,
                               const QColor &port_color)
    : PortAppearance(port_location, port_label, port_color, kDefaultPortRadius)
{
}

PortAppearance::PortAppearance(PortLocation port_location, const QString &port_label,
                               const QColor &port_color, double port_radius)
    : m_port_location(port_location)
    , m_port_label(port_label)
    , m_port_color(port_color)
    , m_port_radius(port_radius)
{
  CalculateBoundingRect();
}

PortLocation PortAppearance::GetPortLocation() const
{
  return m_port_location;
}

QString PortAppearance::GetPortLabel() const
{
  return m_port_label;
}

QColor PortAppearance::GetPortColor() const
{
  return m_port_color;
}

QRectF PortAppearance::GetBoundingRect() const
{
  return m_bounding_rect;
}

double PortAppearance::GetPortRadius() const
{
  return m_port_radius;
}

void PortAppearance::CalculateBoundingRect()
{
  // FIXME implement properly taking into accound port circle and label
  const QPointF top_left{0.0, 0.0};
  const QSizeF size{20.0, 20.0};
  m_bounding_rect = QRectF{top_left, size};
}

bool operator==(const PortAppearance &lhs, const PortAppearance &rhs)
{
  return (lhs.GetPortLocation() == rhs.GetPortLocation())
         && (lhs.GetPortLabel() == rhs.GetPortLabel())
         && (lhs.GetPortColor() == rhs.GetPortColor());
}

bool operator!=(const PortAppearance &lhs, const PortAppearance &rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
