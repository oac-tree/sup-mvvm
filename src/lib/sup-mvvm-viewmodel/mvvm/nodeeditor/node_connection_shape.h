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
#include <mvvm/nodeeditor/node_editor_types.h>
#include <mvvm/nodeeditor/node_connection_style.h>

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
  NodeConnectionShape(NodeTreeLayout tree_layout, std::unique_ptr<INodeConnection> node_connection);
  ~NodeConnectionShape() override;

  INodeConnection* GetNodeConnection();

  void UpdatePath();

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  /**
   * @brief Checks if given connection is hanging (not connected to both ports).
   */
  bool IsHanging() const;

private:
  NodeTreeLayout m_tree_layout;
  std::unique_ptr<INodeConnection> m_node_connection;
  NodeConnectionStyle m_connection_style;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_SHAPE_H_
