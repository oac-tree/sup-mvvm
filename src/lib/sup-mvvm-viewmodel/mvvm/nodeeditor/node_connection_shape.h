/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <QGraphicsItem>
#include <memory>

namespace mvvm
{

class INodeConnection;
class NodeConnectionPainter;

/**
 * @brief The NodeConnectionShape is a graphics object representing a Bezier curve connectind two
 * nodes.
 *
 * The connection logic is extracted to INodeConnection, the painting logic is handled by the
 * NodeConnectionPainter class.
 */
class NodeConnectionShape : public QGraphicsItem
{
public:
  NodeConnectionShape(NodeTreeLayout tree_layout, std::unique_ptr<INodeConnection> node_connection);
  ~NodeConnectionShape() override;

  NodeConnectionShape(const NodeConnectionShape&) = delete;
  NodeConnectionShape& operator=(const NodeConnectionShape&) = delete;
  NodeConnectionShape(NodeConnectionShape&&) = delete;
  NodeConnectionShape& operator=(NodeConnectionShape&&) = delete;

  QRectF boundingRect() const override;

  QPainterPath shape() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  INodeConnection* GetNodeConnection();

  /**
   * @brief Update path from current position of ports.
   *
   * This is a heavy function, that will calculate a path, bounding box and shape (for hover in,
   * hover out events).
   */
  void UpdatePath(const NodeOperationStates& operation_states);

  /**
   * @brief Checks if given connection is hanging (not connected to both ports).
   */
  bool IsHanging() const;

  /**
   * @brief Returns path representing given connection.
   */
  QPainterPath GetPainterPath() const;

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
  QPainterPath CalculatePainterPath() const;

  NodeTreeLayout m_tree_layout;
  std::unique_ptr<INodeConnection> m_node_connection;
  std::unique_ptr<NodeConnectionPainter> m_shape_painter;
  QPainterPath m_path;
  QRectF m_bounding_rect;
  QPainterPath m_shape;
  NodeOperationStates m_current_states;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_SHAPE_H_
