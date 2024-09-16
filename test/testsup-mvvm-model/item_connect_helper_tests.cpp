/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/session_item.h"
#include "mvvm/signals/item_connect_helper.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class ItemConnectHelperTest : public ::testing::Test
{
};

//! Testing method GetEventSource for all type of events.
TEST_F(ItemConnectHelperTest, GetEventSource)
{
  SessionItem item;
  const TagIndex tagindex{"abc", 1};
  SessionModel model;

  EXPECT_EQ(GetEventSource(DataChangedEvent{&item, 42}), &item);
  EXPECT_EQ(GetEventSource(PropertyChangedEvent{&item, "abc"}), &item);
  EXPECT_EQ(GetEventSource(AboutToInsertItemEvent{&item, tagindex}), &item);
  EXPECT_EQ(GetEventSource(ItemInsertedEvent{&item, tagindex}), &item);
  EXPECT_EQ(GetEventSource(AboutToRemoveItemEvent{&item, tagindex}), &item);
  EXPECT_EQ(GetEventSource(ItemRemovedEvent{&item, tagindex}), &item);
  EXPECT_EQ(GetEventSource(ModelAboutToBeResetEvent{&model}), nullptr);
  EXPECT_EQ(GetEventSource(ModelResetEvent{&model}), nullptr);
  EXPECT_EQ(GetEventSource(ModelAboutToBeDestroyedEvent{&model}), nullptr);
}

//! Testing utility function GetEventHandler.
TEST_F(ItemConnectHelperTest, GetEventHandler)
{
  EXPECT_THROW(GetEventHandler(nullptr), NullArgumentException);

  // item without the model doesn't have an event handler
  const SessionItem item;
  EXPECT_THROW(GetEventHandler(&item), NullArgumentException);

  // item belonging to SessionModel doesn't have an event handler
  const SessionModel model;
  EXPECT_THROW(GetEventHandler(model.GetRootItem()), LogicErrorException);

  // item belonging to ApplicationModel does have an event handler
  const ApplicationModel application_model;
  EXPECT_NO_THROW(GetEventHandler(application_model.GetRootItem()));
  EXPECT_NE(GetEventHandler(application_model.GetRootItem()), nullptr);
}

//! Testing utility function ConvertToPropertyChangedEvent.
TEST_F(ItemConnectHelperTest, ConvertToPropertyChangedEvent)
{
  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  auto& property_item = compound->AddProperty("height", 42);

  DataChangedEvent event{&property_item, DataRole::kData};

  {  // successfull convertion
    auto result = ConvertToPropertyChangedEvent(compound, event_variant_t(event));
    EXPECT_TRUE(result.has_value());

    const PropertyChangedEvent expected_converted_event{compound, "height"};
    EXPECT_EQ(result.value(), expected_converted_event);
  }

  {  // wrong convertion
    // DataChangedEvent happened with property_item can't be PropertyChangedEvent of the same item
    auto result = ConvertToPropertyChangedEvent(&property_item, event_variant_t(event));
    EXPECT_FALSE(result.has_value());
  }
}
