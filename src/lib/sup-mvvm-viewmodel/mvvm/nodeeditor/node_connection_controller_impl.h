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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_IMPL_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_IMPL_H_

#include <mvvm/nodeeditor/node_connection_controller_context.h>
#include <mvvm/nodeeditor/node_editor_types.h>

#include <QGraphicsScene>
#include <memory>

namespace mvvm
{

class NodeConnectionShape;
class ConnectableShape;
class INodePort;
class NodePortShape;

/**
 * @brief The NodeConnectionControllerImpl class assists NodeConnectionController in establishing a
 * connection between two ports.
 */
class NodeConnectionControllerImpl
{
public:
  NodeConnectionControllerImpl(const NodeConnectionControllerContext& context,
                                        QGraphicsScene* scene);
  ~NodeConnectionControllerImpl();

  NodeConnectionControllerImpl(const NodeConnectionControllerImpl&) = delete;
  NodeConnectionControllerImpl& operator=(const NodeConnectionControllerImpl&) = delete;
  NodeConnectionControllerImpl(NodeConnectionControllerImpl&&) = delete;
  NodeConnectionControllerImpl& operator=(NodeConnectionControllerImpl&&) = delete;

  /**
   * @brief Finds NodePort around given coordinate.
   */
  NodePortShape* FindNodePortShape(const QPointF& scene_pos);

  EventProcessStatus ProcessMousePress(const QPointF& scene_pos, Qt::MouseButton button);
  EventProcessStatus ProcessMouseMove(const QPointF& scene_pos);
  EventProcessStatus ProcessMouseRelease(const QPointF& scene_pos, Qt::MouseButton button);

  NodeConnectionShape* GetCurrentConnection() const;

INodePort* GetEndPortProxy() const;

private:
  QGraphicsScene* m_scene{nullptr};
  NodeConnectionControllerContext m_context;
  std::unique_ptr<NodeConnectionShape> m_current_connection;
  //!< A proxy port to follow mouse cursor during connection.
  std::unique_ptr<INodePort> m_end_port_proxy;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_
