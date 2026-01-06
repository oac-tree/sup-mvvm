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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QObject>
#include <memory>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;

namespace mvvm
{

class NodeConnectionGuideImpl;
class INodePort;
struct NodeConnectionGuideContext;

/**
 * @brief The NodeConnectionGuide class assists GraphicsScene in painting a connection
 * between two ports.
 */
class NodeConnectionGuide : public QObject
{
  Q_OBJECT

public:
  explicit NodeConnectionGuide(QGraphicsScene* scene);
  ~NodeConnectionGuide() override;

  NodeConnectionGuide(const NodeConnectionGuide&) = delete;
  NodeConnectionGuide& operator=(const NodeConnectionGuide&) = delete;
  NodeConnectionGuide(NodeConnectionGuide&&) = delete;
  NodeConnectionGuide& operator=(NodeConnectionGuide&&) = delete;

  bool eventFilter(QObject* watched, QEvent* event) override;

signals:
  void connectionRequest(const mvvm::INodePort* start_port, const mvvm::INodePort* end_port);
  void connectionStarted();
  void connectionFinished();

private:
  NodeConnectionGuideContext CreateContext();
  std::unique_ptr<NodeConnectionGuideImpl> m_guide_impl;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_H_
