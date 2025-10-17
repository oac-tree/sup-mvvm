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

namespace mvvm
{

PortAppearance::PortAppearance(PortLocation port_location, const QString &port_label)
    : PortAppearance(port_location, port_label, NodePortStyle{})
{
}

PortAppearance::PortAppearance(PortLocation port_location, const QString &port_label,
                               const NodePortStyle &port_style)
    : m_port_location(port_location), m_port_label(port_label), m_port_style(port_style)
{
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
  return GetPortStyle().base_color;
}

double PortAppearance::GetPortRadius() const
{
  return GetPortStyle().radius;
}

const NodePortStyle &PortAppearance::GetPortStyle() const
{
  return m_port_style;
}

bool operator==(const PortAppearance &lhs, const PortAppearance &rhs)
{
  return (lhs.GetPortLocation() == rhs.GetPortLocation())
         && (lhs.GetPortLabel() == rhs.GetPortLabel());
}

bool operator!=(const PortAppearance &lhs, const PortAppearance &rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
