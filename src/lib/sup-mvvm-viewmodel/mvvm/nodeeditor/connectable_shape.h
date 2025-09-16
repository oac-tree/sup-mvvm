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

#ifndef MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_
#define MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_

#include <mvvm/nodeeditor/node_editor_types.h>

#include <QGraphicsItem>
#include <memory>

namespace mvvm
{

class IConnectableShapeAdapter;
class IPortPositionStrategy;
class PortInfo;
class ItemListener;
class INodePort;

/**
 * @brief The ConnectableShape is a graphics object which have ports for connecting other objects.
 */
class ConnectableShape : public QGraphicsItem
{
public:
  explicit ConnectableShape(std::unique_ptr<IConnectableShapeAdapter> shape_adapter,
                            std::unique_ptr<IPortPositionStrategy> port_position_strategy,
                            QGraphicsItem* parent_item = nullptr);
  ~ConnectableShape() override;

  ConnectableShape(const ConnectableShape&) = delete;
  ConnectableShape& operator=(const ConnectableShape&) = delete;
  ConnectableShape(ConnectableShape&&) = delete;
  ConnectableShape& operator=(ConnectableShape&&) = delete;

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  /**
   * @brief Returns exact rectangle of the shape without margins.
   */
  static QRectF GetShapeRect();

  /**
   * @brief Updates item properties from the view position.
   */
  void UpdateItemFromView();

  /**
   * @brief Updates view properties POsition, color) from the item.
   */
  void UpdateViewFromItem();

  /**
   * @brief Makes the given child shape connected to this shape.
   *
   * This method creates a connection between this shape and the provided child shape.
   */
  void MakeChildConnected(ConnectableShape* child_shape);

  /**
   * @brief Returns all ports of this shape.
   */
  std::vector<INodePort*> GetPorts() const;

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
  void CalculateBoundingBox();

  /**
   * @brief Creates input and output ports according to the shape adapter information.
   */
  void SetupPorts();

  /**
   * @brief Creates port shapes for the given direction using provided port infos.
   */
  void SetupPortsForDirection(const std::vector<PortInfo>& port_infos,
                              PortDirection port_direction);

  void SetupItemPropertyListener();

  /**
   * @brief Creates a port shape for the given port info.
   */
  std::unique_ptr<QGraphicsItem> CreateNodePortShape(const PortInfo& port_info);

  std::unique_ptr<IConnectableShapeAdapter> m_shape_adapter;
  std::unique_ptr<IPortPositionStrategy> m_port_position_strategy;
  QRectF m_bounding_box;
  bool m_block_view_update{false};
  bool m_block_item_update{false};
  std::unique_ptr<ItemListener> m_item_listener;
  std::vector<INodePort*> m_node_ports;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_
