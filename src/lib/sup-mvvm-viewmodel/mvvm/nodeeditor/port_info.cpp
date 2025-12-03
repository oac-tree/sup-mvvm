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
    : PortInfo(port_direction, std::move(port_type), {}, nullptr)
{
}

PortInfo::PortInfo(PortDirection port_direction, std::string port_type, std::string port_label,
                   SessionItem* origin)
    : m_port_direction(port_direction)
    , m_port_type(std::move(port_type))
    , m_port_label(std::move(port_label))
    , m_origin(origin)
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

std::string PortInfo::GetPortLabel() const
{
  return m_port_label;
}

SessionItem* PortInfo::GetOrigin() const
{
  return m_origin;
}

bool operator==(const PortInfo& lhs, const PortInfo& rhs)
{
  return (lhs.GetPortDirection() == rhs.GetPortDirection())
         && (lhs.GetPortType() == rhs.GetPortType());
}

bool operator!=(const PortInfo& lhs, const PortInfo& rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
