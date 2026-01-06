/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/signals/item_event_visitor.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/session_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

class ItemEventVisitorTest : public ::testing::Test
{
};

TEST_F(ItemEventVisitorTest, GetEventSource)
{
  const TagIndex tagindex{"abc", 1};
  SessionModel model;

  ItemEventVisitor visitor;

  {
    SessionItem item;
    visitor(DataChangedEvent{&item, 42});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(PropertyChangedEvent{&item, "abc"});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(AboutToInsertItemEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(AboutToInsertItemEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(ItemInsertedEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(ItemInsertedEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }
  {
    SessionItem item;
    visitor(AboutToRemoveItemEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    SessionItem item;
    visitor(ItemRemovedEvent{&item, tagindex});
    EXPECT_EQ(visitor.m_source, &item);
  }

  {
    visitor(ModelAboutToBeResetEvent{&model});
    EXPECT_EQ(visitor.m_source, nullptr);
  }

  {
    visitor(ModelResetEvent{&model});
    EXPECT_EQ(visitor.m_source, nullptr);
  }

  {
    visitor(ModelAboutToBeDestroyedEvent{&model});
    EXPECT_EQ(visitor.m_source, nullptr);
  }
}

}  // namespace mvvm::test
