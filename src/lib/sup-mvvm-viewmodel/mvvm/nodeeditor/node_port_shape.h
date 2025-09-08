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
#include <mvvm/nodeeditor/port_appearance.h>

#include <QGraphicsItem>
#include <memory>

class QGraphicsTextItem;

namespace mvvm
{

class INodePort;

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

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
  PortAppearance m_port_appearance;
  std::unique_ptr<INodePort> m_node_port;
  QGraphicsTextItem* m_label{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_PORT_SHAPE_H_
