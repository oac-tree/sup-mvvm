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
#include "mvvm/model/session_model.h"
#include "mvvm/signals/event_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class EventUtilsTests : public ::testing::Test
{
};

//! Testing method GetEventSource for all type of events.

TEST_F(EventUtilsTests, GetEventSource)
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
