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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_SHAPE_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_SHAPE_H_

#include <mvvm/nodeeditor/i_node_connection.h>

#include <QGraphicsPathItem>
#include <memory>

namespace mvvm
{

class INodeConnection;

/**
 * @brief The NodeConnectionShape is a graphics object representing a Bezier curve connectind two
 * nodes.
 */
class NodeConnectionShape : public QGraphicsPathItem
{
public:
  NodeConnectionShape(std::unique_ptr<INodeConnection> node_connection, QGraphicsItem* parent);

private:
  std::unique_ptr<INodeConnection> m_node_connection;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_SHAPE_H_
