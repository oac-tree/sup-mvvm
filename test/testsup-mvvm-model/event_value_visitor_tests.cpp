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

#include "mvvm/signals/event_value_visitor.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for EventValueVisitor class.
 */
class EventValueVisitorTest : public ::testing::Test
{
public:
  bool Contains(const std::string& str, const std::string& pattern)
  {
    return str.find(pattern) != std::string::npos;
  }
};

TEST_F(EventValueVisitorTest, EventToString)
{
  EXPECT_TRUE(Contains(EventToString(DataChangedEvent{}), "DataChangedEvent"));
  EXPECT_TRUE(Contains(EventToString(PropertyChangedEvent{}), "PropertyChangedEvent"));
  EXPECT_TRUE(Contains(EventToString(AboutToInsertItemEvent{}), "AboutToInsertItemEvent"));
  EXPECT_TRUE(Contains(EventToString(ItemInsertedEvent{}), "ItemInsertedEvent"));
  EXPECT_TRUE(Contains(EventToString(AboutToRemoveItemEvent{}), "AboutToRemoveItemEvent"));
  EXPECT_TRUE(Contains(EventToString(ItemRemovedEvent{}), "ItemRemovedEvent"));
  EXPECT_TRUE(Contains(EventToString(ItemDestroyedEvent{}), "ItemDestroyedEvent"));
  EXPECT_TRUE(Contains(EventToString(ModelAboutToBeResetEvent{}), "ModelAboutToBeResetEvent"));
  EXPECT_TRUE(Contains(EventToString(ModelResetEvent{}), "ModelResetEvent"));
  EXPECT_TRUE(
      Contains(EventToString(ModelAboutToBeDestroyedEvent{}), "ModelAboutToBeDestroyedEvent"));
}

}  // namespace mvvm::test
