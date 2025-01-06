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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/test/mock_item_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class MockItemController : public mvvm::ItemController<mvvm::SessionItem>
{
public:
  MockItemController() = default;
  explicit MockItemController(SessionItem* item) { SetItem(item); }

  MOCK_METHOD(void, OnSubscribe, ());
  MOCK_METHOD(void, OnUnsubscribe, ());
  MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));

protected:
  void Subscribe() override
  {
    Listener()->Connect<mvvm::ItemInsertedEvent>(this, &MockItemController::OnEvent);
    Listener()->Connect<mvvm::AboutToRemoveItemEvent>(this, &MockItemController::OnEvent);
    Listener()->Connect<mvvm::ItemRemovedEvent>(this, &MockItemController::OnEvent);
    Listener()->Connect<mvvm::DataChangedEvent>(this, &MockItemController::OnEvent);
    Listener()->Connect<mvvm::PropertyChangedEvent>(this, &MockItemController::OnEvent);

    OnSubscribe();
  };

  void Unsubscribe() override { OnUnsubscribe(); };
};

class ItemControllerTest : public ::testing::Test
{
public:
  using mock_controller_t = ::testing::StrictMock<MockItemController>;
};

TEST_F(ItemControllerTest, InitialState)
{
  const ItemController<SessionItem> controller;
  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_EQ(controller.Listener(), nullptr);
}

TEST_F(ItemControllerTest, SetItem)
{
  mock_controller_t controller;

  ApplicationModel app_model;
  auto item1 = app_model.InsertItem<PropertyItem>();
  auto item2 = app_model.InsertItem<PropertyItem>();

  EXPECT_CALL(controller, OnSubscribe()).Times(1);

  controller.SetItem(item1);
  EXPECT_EQ(controller.GetItem(), item1);
  EXPECT_NE(controller.Listener(), nullptr);

  controller.SetItem(item1);  // setting the same again, no calls expected

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(controller, OnUnsubscribe()).Times(1);
    EXPECT_CALL(controller, OnSubscribe()).Times(1);
  }

  controller.SetItem(item2);
  EXPECT_EQ(controller.GetItem(), item2);
  EXPECT_NE(controller.Listener(), nullptr);

  EXPECT_CALL(controller, OnUnsubscribe()).Times(1);

  controller.SetItem(nullptr);
  EXPECT_EQ(controller.GetItem(), nullptr);
  EXPECT_EQ(controller.Listener(), nullptr);
}

//! Single call OnDataChanged expected when data was changed.
TEST_F(ItemControllerTest, OnDataChangedSubscribeTwice)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  const auto expected_role = DataRole::kData;
  item->SetData(42, expected_role);

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribe()).Times(1);
  controller.SetItem(item);

  controller.SetItem(item);  // intenionally set item second time to see that no double subscription
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(controller, OnEvent(event_variant_t{expected_event})).Times(1);

  // trigger calls
  item->SetData(45, expected_role);

  // disconnect
  EXPECT_CALL(controller, OnUnsubscribe()).Times(1);
  controller.SetItem(nullptr);

  item->SetData(46, expected_role);
  // expect no calls here
}
