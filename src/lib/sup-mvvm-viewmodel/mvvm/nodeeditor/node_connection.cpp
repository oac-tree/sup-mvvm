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

#include "node_connection.h"

namespace mvvm
{

const INodePort *NodeConnection::GetStartPort() const
{
  return m_start_port;
}

void NodeConnection::SetStartPort(const INodePort *port)
{
  m_start_port = port;
}

const INodePort *NodeConnection::GetEndPort() const
{
  return m_end_port;
}

void NodeConnection::SetEndPort(const INodePort *port)
{
  m_end_port = port;
}

bool NodeConnection::HasPort(const INodePort *port) const
{
  if (port == nullptr)
  {
    return false;
  }

  return m_start_port == port || m_end_port == port;
}

}  // namespace mvvm
