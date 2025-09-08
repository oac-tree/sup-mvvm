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

PortAppearance::PortAppearance(PortLocation port_location, std::string port_label,
                               std::string port_color)
    : m_port_location(port_location)
    , m_port_label(std::move(port_label))
    , m_port_color(std::move(port_color))
{
}

PortLocation PortAppearance::GetPortLocation() const
{
  return m_port_location;
}

std::string PortAppearance::GetPortLabel() const
{
  return m_port_label;
}

std::string PortAppearance::GetPortColor() const
{
  return m_port_color;
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
