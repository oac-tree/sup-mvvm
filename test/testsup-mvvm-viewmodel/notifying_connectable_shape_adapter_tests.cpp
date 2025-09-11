/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/nodeeditor/notifying_connectable_shape_adapter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/node_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <gtest/gtest.h>
#include <testutils/mock_connectable_shape_adapter.h>

namespace mvvm::test
{

/**
 * @brief Tests NotifyingConnectableShapeAdapter class.
 */
class NotifyingConnectableShapeAdapterTest : public ::testing::Test
{
public:
  NotifyingConnectableShapeAdapterTest()
      : m_node_item(m_app_model.InsertItem<NodeItem>()), m_mock_item_listener(m_node_item)
  {
  }

  std::unique_ptr<NotifyingConnectableShapeAdapter> CreateAdapter()
  {
    ON_CALL(m_mock_shape_adapter, GetItem()).WillByDefault(::testing::Return(m_node_item));

    auto decoratee = std::make_unique<ConnectableShapeDecorator>(&m_mock_shape_adapter);

    EXPECT_CALL(m_mock_shape_adapter, GetItem()).Times(1);

    return std::make_unique<NotifyingConnectableShapeAdapter>(
        std::move(decoratee), m_mock_update_callback.AsStdFunction());
  }

  ApplicationModel m_app_model;
  NodeItem* m_node_item{nullptr};

  ::testing::StrictMock<MockConnectableShapeAdapter> m_mock_shape_adapter;
  ::testing::StrictMock<MockItemListener> m_mock_item_listener;
  ::testing::MockFunction<void()> m_mock_update_callback;
};

TEST_F(NotifyingConnectableShapeAdapterTest, CheckMethodForward)
{
  auto adapter = CreateAdapter();
  EXPECT_CALL(m_mock_shape_adapter, GetItem()).Times(1);
  adapter->GetItem();
  EXPECT_CALL(m_mock_shape_adapter, GetDisplayName()).Times(1);
  adapter->GetDisplayName();
  EXPECT_CALL(m_mock_shape_adapter, GetBaseColor()).Times(1);
  adapter->GetBaseColor();
  EXPECT_CALL(m_mock_shape_adapter, GetX()).Times(1);
  adapter->GetX();
  EXPECT_CALL(m_mock_shape_adapter, SetX(42.0)).Times(1);
  adapter->SetX(42.0);
  EXPECT_CALL(m_mock_shape_adapter, GetY()).Times(1);
  adapter->GetY();
  EXPECT_CALL(m_mock_shape_adapter, SetY(43.0)).Times(1);
  adapter->SetY(43.0);
  EXPECT_CALL(m_mock_shape_adapter, GetPortInfos()).Times(1);
  adapter->GetPortInfos();
}

TEST_F(NotifyingConnectableShapeAdapterTest, SetCoordinatesViAdapter)
{
  auto adapter = CreateAdapter();

  // When we set coordinates via dapter, there are no callbacks going back

  EXPECT_CALL(m_mock_update_callback, Call()).Times(0);
  EXPECT_CALL(m_mock_shape_adapter, SetX(42.0)).Times(1);
  adapter->SetX(42.0);
  EXPECT_CALL(m_mock_shape_adapter, SetY(43.0)).Times(1);
  adapter->SetY(43.0);
}

TEST_F(NotifyingConnectableShapeAdapterTest, SetCoordinatesViaItem)
{
  auto adapter = CreateAdapter();
  EXPECT_CALL(m_mock_update_callback, Call()).Times(1);
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(::testing::_)).Times(1);
  m_node_item->SetX(42.0);

  EXPECT_CALL(m_mock_update_callback, Call()).Times(1);
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(::testing::_)).Times(1);
  m_node_item->SetY(43.0);

  EXPECT_CALL(m_mock_update_callback, Call()).Times(1);
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(::testing::_)).Times(1);
  m_node_item->SetBaseColor("green");
}

}  // namespace mvvm::test
