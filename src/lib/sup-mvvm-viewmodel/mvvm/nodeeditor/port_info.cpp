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

#include "port_info.h"

namespace mvvm
{

PortInfo::PortInfo(PortDirection port_direction, std::string port_type)
    : m_port_direction(port_direction), m_port_type(std::move(port_type))
{
}

PortDirection PortInfo::GetPortDirection() const
{
  return m_port_direction;
}

std::string PortInfo::GetPortType() const
{
  return m_port_type;
}

bool operator==(const PortInfo &lhs, const PortInfo &rhs)
{
  return (lhs.GetPortDirection() == rhs.GetPortDirection())
         && (lhs.GetPortType() == rhs.GetPortType());
}

bool operator!=(const PortInfo &lhs, const PortInfo &rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
