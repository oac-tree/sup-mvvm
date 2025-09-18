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

#include "mvvm/nodeeditor/node_connection_controller.h"

#include <mvvm/nodeeditor/node_connection.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>

#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(const mvvm::INodePort*)

namespace mvvm::test
{

/**
 * @brief Tests for NodeConnectionController class.
 */
class NodeConnectionControllerTest : public ::testing::Test
{
public:
  NodeConnectionControllerTest() : m_graphics_view(&m_scene)
  {
    m_scene.setSceneRect(0.0, 0.0, 1000, 1000);
    m_graphics_view.setMouseTracking(true);
    m_graphics_view.setFocus();
  }

  std::unique_ptr<NodeConnectionController> CreateController()
  {
    return std::make_unique<NodeConnectionController>(&m_scene);
  }

  QGraphicsScene m_scene;
  QGraphicsView m_graphics_view;
};

TEST_F(NodeConnectionControllerTest, InitialState)
{
  auto controller = CreateController();
  EXPECT_EQ(controller->parent(), &m_scene);
}

TEST_F(NodeConnectionControllerTest, PostMousePressEventForScene)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  // QTest::mouseEvent seems having problems, see https://bugreports.qt.io/browse/QTBUG-5232
  GTEST_SKIP();
#endif

  auto controller = CreateController();

  const QPointF start_scene_pos(10.0, 20.0);
  auto start_port_shape = CreateNodePortShape(PortDirection::kInput);
  m_scene.addItem(start_port_shape.get());
  start_port_shape->setPos(start_scene_pos);

  const QPointF end_scene_pos(100.0, 200.0);
  auto end_port_shape = CreateNodePortShape(PortDirection::kOutput);
  m_scene.addItem(end_port_shape.get());
  end_port_shape->setPos(end_scene_pos);

  QSignalSpy spy_connection_started(controller.get(), &NodeConnectionController::connectionStarted);
  QSignalSpy spy_connection_finished(controller.get(),
                                     &NodeConnectionController::connectionFinished);
  QSignalSpy spy_connection_request(controller.get(), &NodeConnectionController::connectionRequest);

  QTest::mousePress(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                    m_graphics_view.mapFromScene(start_scene_pos));

  EXPECT_EQ(spy_connection_started.size(), 1);

  QTest::mouseEvent(QTest::MouseMove, m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                    m_graphics_view.mapFromScene(end_scene_pos));

  QTest::mouseRelease(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      m_graphics_view.mapFromScene(end_scene_pos));

  EXPECT_EQ(spy_connection_finished.size(), 1);
  EXPECT_EQ(spy_connection_request.size(), 1);
  auto arguments = spy_connection_request.takeFirst();
  ASSERT_EQ(arguments.size(), 2);
  EXPECT_EQ(arguments.at(0).value<const INodePort*>(), start_port_shape->GetNodePort());
  EXPECT_EQ(arguments.at(1).value<const INodePort*>(), end_port_shape->GetNodePort());
}

}  // namespace mvvm::test
