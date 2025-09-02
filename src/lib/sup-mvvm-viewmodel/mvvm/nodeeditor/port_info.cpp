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

PortInfo::PortInfo(const std::string &port_type, const std::string &port_color)
    : m_port_type(port_type), m_port_color(port_color)
{
}

std::string PortInfo::GetPortType() const
{
  return m_port_type;
}

std::string PortInfo::GetPortColor() const
{
  return m_port_color;
}

const PortInfo &GetDefaultPortInfo()
{
  static const PortInfo kPortInfo{"BasicPort", "navajowhite"};
  return kPortInfo;
}

const PortInfo &GetRootPortInfo()
{
  static const PortInfo kPortInfo{"BasicPort", "indianred"};
  return kPortInfo;
}

}  // namespace mvvm
