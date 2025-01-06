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

#include "mvvm/model/session_item.h"
#include "mvvm/model/session_model.h"
#include "mvvm/signals/event_types.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class EventTypesTests : public ::testing::Test
{
public:
  int m_value1{42};
  int m_value2{43};
  std::string m_str1{"24"};
  std::string m_str2{"43"};
  SessionItem m_item1;
  SessionItem m_item2;
  TagIndex m_tagindex1{"abc", 1};
  TagIndex m_tagindex2{"def", 2};
  SessionModel m_model1;
  SessionModel m_model2;
};

TEST_F(EventTypesTests, DataChangedEvent)
{
  DataChangedEvent event1{&m_item1, m_value1};
  DataChangedEvent event2{&m_item1, m_value1};
  DataChangedEvent event3{&m_item1, m_value2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, PropertyChangedEvent)
{
  PropertyChangedEvent event1{&m_item1, m_str1};
  PropertyChangedEvent event2{&m_item1, m_str1};
  PropertyChangedEvent event3{&m_item1, m_str2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, AboutToInsertItemEvent)
{
  AboutToInsertItemEvent event1{&m_item1, m_tagindex1};
  AboutToInsertItemEvent event2{&m_item1, m_tagindex1};
  AboutToInsertItemEvent event3{&m_item1, m_tagindex2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, ItemInsertedEvent)
{
  ItemInsertedEvent event1{&m_item1, m_tagindex1};
  ItemInsertedEvent event2{&m_item1, m_tagindex1};
  ItemInsertedEvent event3{&m_item1, m_tagindex2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, AboutToRemoveItemEvent)
{
  AboutToRemoveItemEvent event1{&m_item1, m_tagindex1};
  AboutToRemoveItemEvent event2{&m_item1, m_tagindex1};
  AboutToRemoveItemEvent event3{&m_item1, m_tagindex2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, ItemRemovedEvent)
{
  ItemRemovedEvent event1{&m_item1, m_tagindex1};
  ItemRemovedEvent event2{&m_item1, m_tagindex1};
  ItemRemovedEvent event3{&m_item1, m_tagindex2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, ModelAboutToBeResetEvent)
{
  ModelAboutToBeResetEvent event1{&m_model1};
  ModelAboutToBeResetEvent event2{&m_model1};
  ModelAboutToBeResetEvent event3{&m_model2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, ModelResetEvent)
{
  ModelResetEvent event1{&m_model1};
  ModelResetEvent event2{&m_model1};
  ModelResetEvent event3{&m_model2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(EventTypesTests, ModelAboutToBeDestroyedEvent)
{
  ModelAboutToBeDestroyedEvent event1{&m_model1};
  ModelAboutToBeDestroyedEvent event2{&m_model1};
  ModelAboutToBeDestroyedEvent event3{&m_model2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}
