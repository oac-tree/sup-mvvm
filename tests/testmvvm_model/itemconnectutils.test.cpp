/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/itemconnectutils.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/signals/signalslot.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;
using ::testing::_;

class ItemConnectUtilsTest : public ::testing::Test
{
public:
  class MockWidget
  {
  public:
    explicit MockWidget(SessionItem* item) : m_slot(std::make_unique<Slot>()) { SetItem(item); }

    void SetItem(mvvm::SessionItem* item)
    {
      auto on_data_change = [this](SessionItem* item, int role) { OnDataChanged(item, role); };
      connect::OnDataChange(item, on_data_change, m_slot.get());
    }

    //      MOCK_METHOD2(onItemInserted, void(SessionItem* item, TagIndex tagindex));
    //      MOCK_METHOD2(onAboutToRemoveItem, void(SessionItem* item, TagIndex tagindex));
    //      MOCK_METHOD2(onItemRemoved, void(SessionItem* item, TagIndex tagindex));

    MOCK_METHOD2(OnDataChanged, void(SessionItem* item, int role));
    //      MOCK_METHOD2(onPropertyChanged, void(SessionItem* item, std::string name));

    std::unique_ptr<Slot> m_slot;
  };
};

//! Initialisation of the connection with wrong type of the model.

TEST_F(ItemConnectUtilsTest, OnDataChangeWrongModel)
{
  auto on_data_change = [this](SessionItem* item, int role) {};

  SessionItem item1;
  EXPECT_THROW(connect::OnDataChange(&item1, on_data_change), std::runtime_error);

  SessionModel model;
  auto item2 = model.InsertItem<SessionItem>();
  EXPECT_THROW(connect::OnDataChange(item2, on_data_change), std::runtime_error);

  ApplicationModel application_model;
  auto item3 = application_model.InsertItem<SessionItem>();
  EXPECT_NO_THROW(connect::OnDataChange(item3, on_data_change));
}

//! Single call OnDataChanged expected when data was changed.

TEST_F(ItemConnectUtilsTest, OnDataChange)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  MockWidget widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged after disconnection.

TEST_F(ItemConnectUtilsTest, OnDataChangeAfterDisconnection)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  MockWidget widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);
  item->SetData(45, expected_role);

  // disconnect widget, expect no notifications
  widget.m_slot.reset();
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  item->SetData(46, expected_role);
}

//! Expect no calls OnDataChanged when data is the same.

TEST_F(ItemConnectUtilsTest, OnDataChangeSameData)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  MockWidget widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);
  item->SetData(45, expected_role);

  // same data, expect no notifications
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged when other item is changed.

TEST_F(ItemConnectUtilsTest, OnDataChangeDifferentItem)
{
  ApplicationModel model;
  auto item1 = model.InsertItem<SessionItem>();
  item1->SetData(42, DataRole::kData);
  auto item2 = model.InsertItem<SessionItem>();
  item2->SetData(43, DataRole::kData);

  MockWidget widget(item1);

  // expect no notification
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  item2->SetData(45, DataRole::kData); // other item changed
}
