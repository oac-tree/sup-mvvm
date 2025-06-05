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

#include "mvvm/model/session_item.h"
#include "mvvm/model/session_model.h"
#include "mvvm/signals/event_types.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

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
  const DataChangedEvent event1{&m_item1, m_value1};
  const DataChangedEvent event2{&m_item1, m_value1};
  const DataChangedEvent event3{&m_item1, m_value2};

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
  const PropertyChangedEvent event1{&m_item1, m_str1};
  const PropertyChangedEvent event2{&m_item1, m_str1};
  const PropertyChangedEvent event3{&m_item1, m_str2};

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
  const AboutToInsertItemEvent event1{&m_item1, m_tagindex1};
  const AboutToInsertItemEvent event2{&m_item1, m_tagindex1};
  const AboutToInsertItemEvent event3{&m_item1, m_tagindex2};

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
  const ItemInsertedEvent event1{&m_item1, m_tagindex1};
  const ItemInsertedEvent event2{&m_item1, m_tagindex1};
  const ItemInsertedEvent event3{&m_item1, m_tagindex2};

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
  const AboutToRemoveItemEvent event1{&m_item1, m_tagindex1};
  const AboutToRemoveItemEvent event2{&m_item1, m_tagindex1};
  const AboutToRemoveItemEvent event3{&m_item1, m_tagindex2};

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
  const ItemRemovedEvent event1{&m_item1, m_tagindex1};
  const ItemRemovedEvent event2{&m_item1, m_tagindex1};
  const ItemRemovedEvent event3{&m_item1, m_tagindex2};

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
  const ModelAboutToBeResetEvent event1{&m_model1};
  const ModelAboutToBeResetEvent event2{&m_model1};
  const ModelAboutToBeResetEvent event3{&m_model2};

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
  const ModelResetEvent event1{&m_model1};
  const ModelResetEvent event2{&m_model1};
  const ModelResetEvent event3{&m_model2};

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
  const ModelAboutToBeDestroyedEvent event1{&m_model1};
  const ModelAboutToBeDestroyedEvent event2{&m_model1};
  const ModelAboutToBeDestroyedEvent event3{&m_model2};

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  // comparing different events
  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

}  // namespace mvvm::test
