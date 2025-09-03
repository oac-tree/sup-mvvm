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

#ifndef MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_
#define MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_

#include <mvvm/nodeeditor/i_node_port.h>

#include <QGraphicsPathItem>
#include <memory>

namespace mvvm
{

class INodePort;

/**
 * @brief The NodePortShape is a graphics object representing a NodePort.
 *
 * It looks line a small filled circle with the border.
 */
class NodePortShape : public QGraphicsPathItem
{
public:
  NodePortShape(std::unique_ptr<INodePort> node_port, QGraphicsItem* parent);

private:
  std::unique_ptr<INodePort> m_node_port;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_
