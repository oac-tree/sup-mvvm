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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QObject>
#include <memory>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;

namespace mvvm
{

class NodeConnectionControllerImpl;
class INodePort;
struct NodeConnectionControllerContext;

/**
 * @brief The NodeConnectionController class assists GraphicsScene in establishing a connection
 * between two ports.
 */
class NodeConnectionController : public QObject
{
  Q_OBJECT

public:
  explicit NodeConnectionController(QGraphicsScene* scene);
  ~NodeConnectionController() override;

  NodeConnectionController(const NodeConnectionController&) = delete;
  NodeConnectionController& operator=(const NodeConnectionController&) = delete;
  NodeConnectionController(NodeConnectionController&&) = delete;
  NodeConnectionController& operator=(NodeConnectionController&&) = delete;

  bool eventFilter(QObject* object, QEvent* event) override;

signals:
  void connectionRequest(const mvvm::INodePort* start_port, const mvvm::INodePort* end_port);
  void connectionStarted();
  void connectionFinished();

private:
  NodeConnectionControllerContext CreateContext();
  std::unique_ptr<NodeConnectionControllerImpl> m_controller_impl;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_
