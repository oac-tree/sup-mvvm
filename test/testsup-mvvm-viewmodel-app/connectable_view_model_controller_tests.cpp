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

#include "mvvm/nodeeditor/connectable_view_model_controller.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/default_node_shape_factory.h>
#include <mvvm/nodeeditor/node_connection_shape.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShape class.
 */
class ConnectableViewModelControllerTest : public ::testing::Test
{
public:
  std::unique_ptr<ConnectableViewModelController> CreateController()
  {
    auto factory = std::make_unique<DefaultNodeShapeFactory>();
    auto controller =
        std::make_unique<ConnectableViewModelController>(std::move(factory), &m_scene);
    return controller;
  }

  template <typename ShapeT>
  std::vector<ShapeT*> GetShapes() const
  {
    return utils::CastItems<ShapeT>(m_scene.items());
  }

  ApplicationModel m_app_model;
  QGraphicsScene m_scene;
};

TEST_F(ConnectableViewModelControllerTest, InitialState)
{
  std::unique_ptr<IConnectableShapeFactory> uninitialized_factory;
  EXPECT_THROW(ConnectableViewModelController(std::move(uninitialized_factory), nullptr),
               RuntimeException);
  EXPECT_THROW(ConnectableViewModelController(std::make_unique<DefaultNodeShapeFactory>(), nullptr),
               RuntimeException);

  EXPECT_NO_THROW(CreateController());
  auto controller = CreateController();
  EXPECT_THROW(controller->InitScene(nullptr), RuntimeException);
}

TEST_F(ConnectableViewModelControllerTest, InitSceneForEmptyModel)
{
  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));
  EXPECT_TRUE(m_scene.items().empty());
}

TEST_F(ConnectableViewModelControllerTest, SingleNodeItem)
{
  auto node_item = m_app_model.InsertItem<toyitems::SequenceItem>();

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
}

}  // namespace mvvm::test
