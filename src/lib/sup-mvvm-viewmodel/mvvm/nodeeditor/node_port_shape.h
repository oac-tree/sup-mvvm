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

#ifndef MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_
#define MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_

#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/port_appearance.h>

#include <QGraphicsItem>
#include <memory>

namespace mvvm
{

class INodePort;
class NodePortShapePainter;

/**
 * @brief The NodePortShape is a graphics object representing a NodePort.
 *
 * It looks line a small filled circle with the border.
 */
class NodePortShape : public QGraphicsItem
{
public:
  NodePortShape(PortAppearance port_appearance, std::unique_ptr<INodePort> node_port,
                QGraphicsItem* parent);

  ~NodePortShape() override;

  NodePortShape(const NodePortShape&) = delete;
  NodePortShape& operator=(const NodePortShape&) = delete;
  NodePortShape(NodePortShape&&) = delete;
  NodePortShape& operator=(NodePortShape&&) = delete;

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  INodePort* GetNodePort() const;

  QColor GetPortColor() const;

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
  void CalculateBoundingRect();
  void PropagateSceneCoordinatesToNodePort();

  PortAppearance m_port_appearance;
  std::unique_ptr<INodePort> m_node_port;
  QGraphicsTextItem* m_label{nullptr};
  QRectF m_bounding_rect;
  NodeOperationStates m_operation_states;
  std::unique_ptr<NodePortShapePainter> m_painter;
};

/**
 * @brief Returns a rectangle for label painting.
 *
 * @param port_location The location of the port.
 * @param port_position Port coordinates (normally (0,0))
 * @param label The text of the label.
 * @return The rectangle to plot the text.
 */
QRectF GetLabelRect(PortLocation port_location, const QPointF& port_position, const QString& label);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_
