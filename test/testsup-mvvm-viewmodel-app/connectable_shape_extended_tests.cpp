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

#include "mvvm/nodeeditor/connectable_shape.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/standarditems/node_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShape class, when view is fully constructed and belongs to scene.
 */
class ConnectableShapeExtendedTest : public ::testing::Test
{
public:
  ConnectableShapeExtendedTest()
      : m_node_item(m_app_model.InsertItem<NodeItem>()), m_mock_item_listener(m_node_item)
  {
  }

  /**
   * @brief Returns test object wired with proper connection adapter.
   */
  std::unique_ptr<ConnectableShape> CreateConnectableShape()
  {
    auto adapter = std::make_unique<NodeItemConnectableShapeAdapter>(m_node_item);
    auto position_strategy = std::make_unique<TopBottomPortPositionStrategy>();
    return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
  }

  ApplicationModel m_app_model;
  NodeItem* m_node_item{m_app_model.InsertItem<NodeItem>()};
  QGraphicsScene m_scene;

  ::testing::StrictMock<MockItemListener> m_mock_item_listener;
};

TEST_F(ConnectableShapeExtendedTest, DefaultCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape();
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);
  EXPECT_EQ(m_node_item->GetX(), 0.0);
  EXPECT_EQ(m_node_item->GetY(), 0.0);
}

TEST_F(ConnectableShapeExtendedTest, NonDefaultCoordinatesAfterInsert)
{
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(::testing::_)).Times(2);
  m_node_item->SetX(10.0);
  m_node_item->SetY(20.0);

  auto shape = CreateConnectableShape();
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

TEST_F(ConnectableShapeExtendedTest, ChangeItemCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape();
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  const PropertyChangedEvent expected_event1{m_node_item, std::string{NodeItem::kXpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event1)).Times(1);
  const PropertyChangedEvent expected_event2{m_node_item, std::string{NodeItem::kYpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event2)).Times(1);

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);

  m_node_item->SetX(10.0);
  m_node_item->SetY(20.0);

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

TEST_F(ConnectableShapeExtendedTest, ChangeSceneCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape();
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  const PropertyChangedEvent expected_event1{m_node_item, std::string{NodeItem::kXpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event1)).Times(1);
  const PropertyChangedEvent expected_event2{m_node_item, std::string{NodeItem::kYpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event2)).Times(1);

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);

  shape_ptr->setX(10.0);
  shape_ptr->setY(20.0);

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

}  // namespace mvvm::test
