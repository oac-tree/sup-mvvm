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

#include "node_item_connectable_shape_adapter.h"

#include "node_editor_types.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/node_item.h>

namespace mvvm
{

namespace
{
constexpr std::string_view kDefaultPortType = "kDefaultPortType";
}

NodeItemConnectableShapeAdapter::NodeItemConnectableShapeAdapter(NodeItem *node_item)
    : m_node_item(node_item)
{
  if (!m_node_item)
  {
    throw RuntimeException("Node item is not set");
  }
}

NodeItem *NodeItemConnectableShapeAdapter::GetNodeItem() const
{
  return m_node_item;
}

QString NodeItemConnectableShapeAdapter::GetDisplayName() const
{
  return QString::fromStdString(m_node_item->GetDisplayName());
}

QColor NodeItemConnectableShapeAdapter::GetBaseColor() const
{
  return {QString::fromStdString(m_node_item->GetBaseColor())};
}

double NodeItemConnectableShapeAdapter::GetX() const
{
  return m_node_item->GetX();
}

void NodeItemConnectableShapeAdapter::SetX(double value)
{
  m_node_item->SetX(value);
}

double NodeItemConnectableShapeAdapter::GetY() const
{
  return m_node_item->GetY();
}

void NodeItemConnectableShapeAdapter::SetY(double value)
{
  m_node_item->SetY(value);
}

std::vector<PortInfo> NodeItemConnectableShapeAdapter::GetPortInfos() const
{
  const PortInfo output_port(PortDirection::kOutput, std::string(kDefaultPortType), "out");

  // items which can have children will have input port as well
  if (utils::HasTag(*m_node_item, constants::kChildrenTag))
  {
    const PortInfo input_port(PortDirection::kInput, std::string(kDefaultPortType), "in");
    return {input_port, output_port};
  }

  // items always have output port by default
  return {output_port};
}

}  // namespace mvvm
